/**
 * The code initally seem to work fine. The implementation of mutex and semaphore made the code more robust and thread-safe,
 * ensuring that the shared resource (job_count) is accessed in a controlled manner.
 * 
 * The semaphore is used to signal the printer thread when a new job is added, allowing it to process jobs as they come in.
 * It works as a counter for the number of print jobs, ensuring that the printer thread only processes when there are jobs available.
 * **/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int job_count = 0; // Shared count of print jobs
pthread_mutex_t mutex;
sem_t job_semaphore;

void* user(void* arg) {
    for (int i = 0; i < 3; i++) {
        pthread_mutex_lock(&mutex);
        job_count++;
        pthread_mutex_unlock(&mutex);
        printf("User %ld added a print job. Total jobs: %d\n", (long)arg, job_count);
        sem_post(&job_semaphore);
    }

    return NULL;
}

void* printer(void* arg) {
    for (int i = 0; i < 9; i++) { // Assumes 3 users with 3 jobs each
        sem_wait(&job_semaphore);
        if (job_count > 0) { // The if statement might not be necessary (AKA redundant)
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
    sem_init(&job_semaphore, 0, 0);

    pthread_t users[3], printer_thread;

    for (long i = 0; i < 3; i++) {
        pthread_create(&users[i], NULL, user, (void*)i);
    }
    pthread_create(&printer_thread, NULL, printer, NULL);
    
    for (int i = 0; i < 3; i++) {
        pthread_join(users[i], NULL);
    }
    pthread_join(printer_thread, NULL);
    
    sem_destroy(&job_semaphore);
    pthread_mutex_destroy(&mutex);
    return 0;
}