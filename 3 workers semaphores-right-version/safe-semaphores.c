#include "safe-semaphores.h"

pid_t fork_safe(void) {
    pid_t p = fork();
    if (p < 0) {
        perror("fork creation fail");
        exit(EXIT_FAILURE);
    }
    return p;
}

int Shmget_safe(size_t size) {
    int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);  // IPC_CREAT to create if not exists
    if (shmid < 0) {
        perror("shmget fail");
        exit(EXIT_FAILURE);
    }
    return shmid;
}

void* Shmat_safe(int shmid) {
    void *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (void *) -1) {
        perror("shmat fail");
        exit(EXIT_FAILURE);
    }
    return shm_ptr;
}

int GetVal_safe(int semid, int num_in_sem) {
    
    int return_value = semctl(semid, num_in_sem, GETVAL);
    if (return_value < 0) {
        perror("semctl GetVal fail\n");
        exit(EXIT_FAILURE);
    }
    return return_value;
}


void SetVal_safe(int semid, int num_in_sem, int value) {
    
    if (semctl(semid, num_in_sem, SETVAL, value) < 0) {
        perror("semctl SetVal fail\n");
        exit(EXIT_FAILURE);
    }
}


void RunOp_safe(int semid, struct sembuf *op, size_t nop) {
    
    if (semop(semid, op, nop) < 0) {
        perror("semop RunOp fail");
        exit(EXIT_FAILURE);
    }
}


int create_semaphore(const char* name, int flags) {

    int semid = semget(ftok(name, 1), 4, flags);
    if (semid == -1) {
        perror("semaphore creation fail\n");
        exit(EXIT_FAILURE);
    }
    
    SetVal_safe(semid, 0, 2);  // A = 2
    SetVal_safe(semid, 1, 1);  // B = 1
    SetVal_safe(semid, 2, 1);  // MUTEX = 1
    SetVal_safe(semid, 3, 0);  // Ready = 0

    printf("Semaphore is created\n");
    return semid;
}

void delete_semaphore(int semid) {
    
    if (semctl(semid, IPC_RMID, 0) < 0) {
        perror("semaphore deletion fail\n");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore is deleted!\n");
}