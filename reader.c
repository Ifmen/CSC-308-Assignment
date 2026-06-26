#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_KEY  0x1234
#define SEM_KEY  0x5678
#define SHM_SIZE 1024

void sem_lock(int semid) {
    struct sembuf op = {0, -1, 0};
    semop(semid, &op, 1);
}
void sem_unlock(int semid) {
    struct sembuf op = {0, 1, 0};
    semop(semid, &op, 1);
}

int main() {
    sleep(1); // Let writer create and write first

    // Step 1 & 2: Get and attach existing shared memory
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid < 0) { perror("shmget"); exit(1); }

    char *shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) { perror("shmat"); exit(1); }

    int semid = semget(SEM_KEY, 1, 0666);

    // Step 3: Read data (protected by semaphore)
    sem_lock(semid);
    printf("[Reader] Read from shared memory: \"%s\"\n", shm_ptr);
    sem_unlock(semid);

    // Step 5: Detach
    shmdt(shm_ptr);
    printf("[Reader] Done.\n");

    return 0;
}
