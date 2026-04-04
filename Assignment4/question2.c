#include <stdio.h>
#include <pthread.h>

int job_count = 0; // Shared count of print jobs
pthread_mutex_t mutex;

void* user(void* arg) {
    for (int i = 0; i < 3; i++) {
        pthread_mutex_lock(&mutex);
        job_count++;
        pthread_mutex_unlock(&mutex);
        printf("User %ld added a print job. Total jobs: %d\n", (long)arg, job_count);
    }

    return NULL;
}

void* printer(void* arg) {
    for (int i = 0; i < 9; i++) { // Assumes 3 users with 3 jobs each
        if (job_count > 0) {
            pthread_mutex_lock(&mutex);
            job_count--;
            pthread_mutex_unlock(&mutex);
            printf("Printer processed a job. Remaining jobs: %d\n", job_count);
        } else {
            printf("Printer waiting for jobs.\n");
        }
    }

    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_t users[3], printer_thread;

    for (long i = 0; i < 3; i++) {
        pthread_create(&users[i], NULL, user, (void*)i);
    }
    pthread_create(&printer_thread, NULL, printer, NULL);
    
    for (int i = 0; i < 3; i++) {
        pthread_join(users[i], NULL);
    }
    pthread_join(printer_thread, NULL);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}