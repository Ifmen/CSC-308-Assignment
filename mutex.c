#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5
#define INCREMENT_COUNT 100000

int counter = 0;
pthread_mutex_t mutex;

void *increment_with_mutex(void *arg) {
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *increment_without_mutex(void *arg) {
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        counter++;
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // --- WITH MUTEX ---
    pthread_mutex_init(&mutex, NULL);
    counter = 0;

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, increment_with_mutex, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("WITH mutex:    counter = %d (expected %d)\n",
           counter, NUM_THREADS * INCREMENT_COUNT);

    pthread_mutex_destroy(&mutex);

    // --- WITHOUT MUTEX ---
    counter = 0;

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, increment_without_mutex, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("WITHOUT mutex: counter = %d (expected %d) -- likely WRONG!\n",
           counter, NUM_THREADS * INCREMENT_COUNT);

    return 0;
}
