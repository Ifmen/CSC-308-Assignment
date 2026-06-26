#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS     6
#define INCREMENT_COUNT 100000
#define MAX_CONCURRENT  3

int counter = 0;
pthread_mutex_t mutex;
sem_t binary_sem;
sem_t counting_sem;

void *mutex_increment(void *arg) {
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *semaphore_increment(void *arg) {
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        sem_wait(&binary_sem);
        counter++;
        sem_post(&binary_sem);
    }
    return NULL;
}

void *counting_sem_task(void *arg) {
    long tid = (long)arg;

    sem_wait(&counting_sem);
    printf("  Thread %ld entered critical section\n", tid);
    usleep(300000); // simulate work (0.3s)
    printf("  Thread %ld leaving critical section\n", tid);
    sem_post(&counting_sem);

    return NULL;
}

void benchmark(void *(*fn)(void*), const char *label) {
    pthread_t threads[NUM_THREADS];
    counter = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, fn, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("  [%s] counter = %d | time = %.4fs\n", label, counter, elapsed);
}

int main() {
    printf("=== Binary Exclusion Comparison ===\n");

    pthread_mutex_init(&mutex, NULL);
    sem_init(&binary_sem, 0, 1);

    benchmark(mutex_increment,     "pthread_mutex   ");
    benchmark(semaphore_increment, "binary semaphore");

    pthread_mutex_destroy(&mutex);
    sem_destroy(&binary_sem);

    printf("\n=== Counting Semaphore: Resource Pool (max %d concurrent) ===\n",
           MAX_CONCURRENT);

    sem_init(&counting_sem, 0, MAX_CONCURRENT);
    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, counting_sem_task, (void*)i);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&counting_sem);

    printf("\nKey Takeaway:\n");
    printf("  mutex          = binary exclusion (exactly 1 thread at a time)\n");
    printf("  counting sem   = resource pool (up to N threads simultaneously)\n");
    return 0;
}
