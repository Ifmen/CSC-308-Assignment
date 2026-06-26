#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS   10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t mutex;
sem_t empty;
sem_t full;

void *producer(void *arg) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        int item = i * 10;

        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        printf("[Producer] Produced item: %d | in=%d\n", item, in);

        sem_post(&mutex);
        sem_post(&full);

        usleep(100000); // 0.1s
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        sem_wait(&full);
        sem_wait(&mutex);

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("[Consumer] Consumed item: %d | out=%d\n", item, out);

        sem_post(&mutex);
        sem_post(&empty);

        usleep(200000); // 0.2s - consumer is slower
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full,  0, 0);

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    printf("\nAll items produced and consumed successfully.\n");
    return 0;
}
