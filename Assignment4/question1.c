/**
 * The issue with the given code (old code) was that it did not lock
 * the critical section where the shared variable 'tickets' was being modified.
 * Leading to varibles being read and modified by multiple threads simultaneously,
 * which caused race conditions and incorrect results.
 * 
 * The solution was to use a mutex to lock the critical section of code that modifies
 * the 'tickets' variable. This ensures that only one thread can access and modify the 'tickets'
 **/

#include <stdio.h>
#include <pthread.h>

int tickets = 10; // Shared number of available tickets
pthread_mutex_t mutex;

void* book_tickets(void* arg) {
    for (int i = 0; i < 5; i++) {
        if (tickets > 0) {
            pthread_mutex_lock(&mutex);
            int temp = tickets;
            temp--;
            tickets = temp;
            printf("Booked a ticket, remaining tickets: %d\n", tickets);
            pthread_mutex_unlock(&mutex);
        } else {
            printf("No tickets available to book.\n");
        }
    }

    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, book_tickets, NULL);
    pthread_create(&t2, NULL, book_tickets, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    printf("Final tickets remaining: %d\n", tickets);
    return 0;
}