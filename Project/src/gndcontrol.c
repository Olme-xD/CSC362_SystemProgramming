// This is the main program for the air traffic ground control server.

// The job of this module is to set the system up and then turn over control
// to the airs_protocol module which will handle the actual communication
// protocol between clients (airplanes) and the server.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "airplane.h"
#include "airs_protocol.h"
#include "alist.h"
#include "planelist.h"

/************************************************************************
 * client_thread: This thread will handle communication with a single
 * airplane client. It will read commands from the client and pass them to
 * docommand until the client disconnects or the plane is done.
 */
void *client_thread(void *arg) {
    airplane *plane = (airplane *)arg;
    char *lineptr = NULL;
    size_t linesize = 0;

    // Read commands from the client until they disconnect or the plane is done
    while (plane->state != PLANE_DONE) {
        if (getline(&lineptr, &linesize, plane->fp_recv) < 0) {
            // Failed getline means the client disconnected
            break;
        }
        docommand(plane, lineptr);
    }

    // Free and exit thread
    free(lineptr);
    remove_airplane(plane);
    airplane_destroy(plane);
    free(plane);
    return NULL;
}

/************************************************************************
 * setup_server: This function sets up a TCP server socket on the specified
 * port and returns the file descriptor for the server socket.
 */
int setup_server(int port) {
    int server_file_descriptor;
    struct sockaddr_in address;

    // Create socket file descriptor
    if ((server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed!");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the specified port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port
    if (bind(server_file_descriptor, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed!");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_file_descriptor, 10) < 0) {
        perror("Listen failed!");
        exit(EXIT_FAILURE);
    }

    return server_file_descriptor;
}

/************************************************************************
 * main: The main function initializes the server and enters an infinite loop
 * to accept incoming client connections. For each accepted connection, it
 * creates a new thread to handle communication with that client.
 */
int main(int argc, char *argv[]) {
    // Initialize the global plane list
    init_planelist();

    // Abstracted server setup
    int server_file_descriptor = setup_server(8080);
    printf("Ground Control Server listening on port 8080...\n");

    // Main loop to accept incoming client connections
    while (1) {
        int client_file_descriptor;
        struct sockaddr_in client_address;
        socklen_t address_len = sizeof(client_address);

        // Accept a new client connection
        client_file_descriptor = accept(server_file_descriptor, (struct sockaddr *)&client_address, &address_len);
        if (client_file_descriptor < 0) {
            perror("Accept failed");
            continue; 
        }

        //  the client file descriptor for separate reading and writing
        int client_file_descriptor_dup = dup(client_file_descriptor);
        if (client_file_descriptor_dup < 0) {
            perror("Duplicate failed");
            close(client_file_descriptor);
            continue;
        }

        FILE *fp_recv = fdopen(client_file_descriptor, "r");
        FILE *fp_send = fdopen(client_file_descriptor_dup, "w");
        setvbuf(fp_send, NULL, _IOLBF, 0);
        setvbuf(fp_recv, NULL, _IOLBF, 0);

        // Create a new airplane struct for the client and start a thread to handle it
        airplane *new_plane = malloc(sizeof(airplane));
        if (new_plane == NULL) {
            perror("Memory allocation failed");
            fclose(fp_recv);
            fclose(fp_send);
            continue;
        }
        airplane_init(new_plane, fp_send, fp_recv);
        add_airplane(new_plane);

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_thread, new_plane) != 0) {
            perror("Thread creation failed");

            // If thread fails we delete what it was created
            airplane_destroy(new_plane);
            free(new_plane);
            continue;
        }
        
        pthread_detach(tid);
    }

    return 0;
}
