#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    // Step 1: Create shared memory segment
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) { perror("shmget"); exit(1); }
    printf("[Writer] Shared memory created. shmid = %d\n", shmid);

    // Step 2: Attach shared memory
    char *shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) { perror("shmat"); exit(1); }
    printf("[Writer] Shared memory attached at %p\n", (void*)shm_ptr);

    // Create semaphore
    int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 1);

    // Step 3: Write data (protected by semaphore)
    sem_lock(semid);
    const char *message = "Hello from Writer process via Shared Memory!";
    strncpy(shm_ptr, message, SHM_SIZE);
    printf("[Writer] Written: \"%s\"\n", shm_ptr);
    sem_unlock(semid);

    printf("[Writer] Waiting 3s for reader to consume...\n");
    sleep(3);

    // Step 5: Cleanup
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    printf("[Writer] Shared memory removed. Done.\n");

    return 0;
}
