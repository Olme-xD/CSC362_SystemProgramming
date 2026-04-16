#include <string.h>
#include <pthread.h>

#include "alist.h"
#include "airplane.h"
#include "planelist.h"

pthread_mutex_t planes_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex to protect access to the planes array
alist global_plane_list;  // Array of pointers to airplanes

// Initializes global plane list
void init_planelist(void) {
    alist_init(&global_plane_list, _destructor);
}

// Adds new airplane to global plane list
void add_airplane(airplane *plane) {
    if (plane == NULL) {
        return;
    }

    pthread_mutex_lock(&planes_mutex);
    alist_add(&global_plane_list, plane);
    pthread_mutex_unlock(&planes_mutex);
}

// Removes airplane from global plane list
void remove_airplane(airplane *plane) {
    if (plane == NULL) {
        return;
    }

    // Find plane in the list and removes it
    pthread_mutex_lock(&planes_mutex);
    for (int i = 0; i < alist_size(&global_plane_list); i++) {
        airplane *current_plane = alist_get(&global_plane_list, i);
        
        if (current_plane == plane) {
            alist_remove(&global_plane_list, i);
            break;
        }
    }
    pthread_mutex_unlock(&planes_mutex);
}

// Checks if a flight ID is already taken by another plane
int is_flightid_taken(char *id) {
    pthread_mutex_lock(&planes_mutex);
    for (int i = 0; i < alist_size(&global_plane_list); i++) {
        airplane *p = alist_get(&global_plane_list, i);
        if (strcmp(p->id, id) == 0) {
            pthread_mutex_unlock(&planes_mutex);
            return 1;
        }
    }
    
    pthread_mutex_unlock(&planes_mutex);
    return 0;
}

// Helper destructor for airplane data in the alist
void _destructor(void *data) {}