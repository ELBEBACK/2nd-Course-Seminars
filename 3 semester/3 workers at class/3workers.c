#include "safe-semaphores.h"
#include <sys/wait.h>
#include <stdbool.h>

int* Ready = NULL;

void create_workers(int semid);
void wrench_worker(int semid, int num);
void screwdriver_worker(int semid, int num);
void replace_device();

int devices = 1;

// sem_array = | A = 2 | B = 1 | MUTEX = 1 | Ready = 0 |
int main(void) {
    
    int shmid = Shmget_safe(sizeof(int));
    Ready = Shmat_safe(shmid);
    *Ready = 0;

    int semid = create_semaphore("main.c", IPC_CREAT|0644);
  
    create_workers(semid);

    for (size_t i = 0; i < 3; i++) wait(NULL);

    printf("All processes are ended\n");
    delete_semaphore(semid);
    return 0;
}

void create_workers(int semid) {
    
    for (int i = 1; i <= 2; i++) {
        pid_t pid = fork_safe();
        if (pid == 0) {
        wrench_worker(semid, i);
        exit(EXIT_SUCCESS);
        }
    }
  
    pid_t pid = fork_safe();
    if (pid == 0) {
        screwdriver_worker(semid, 1);
        exit(EXIT_SUCCESS);
    }
}

void wrench_worker(int semid, int num) {
    
    struct sembuf P_A[] = {{0, -1, 0}};        // P(A)
    struct sembuf P_mutex[] = {{2, -1, 0}};    // P(MUTEX)
    struct sembuf V_mutex[] = {{2, 1, 0}};     // V(MUTEX)
    struct sembuf V_A[] = {{0, 1, 0}};         // V(A)
    struct sembuf V_B[] = {{1, 1, 0}};         // V(B)
  
    int details_installed = 0;
  
    while (details_installed < devices) {
        
        RunOp_safe(semid, P_A, 1);               // P(A)
    

        RunOp_safe(semid, P_mutex, 1);           // P(MUTEX)


        printf("Wrench worker %d: installing nut\n", num);
        sleep(1); 
    
        
        *Ready += 1;
    

        if (*Ready + 1 == 3) {
            printf("Wrench worker %d: device completed\n", num);
            replace_device(semid);
        

        RunOp_safe(semid, V_A, 1);             // V(A)
        RunOp_safe(semid, V_A, 1);             // V(A)
        RunOp_safe(semid, V_B, 1);             // V(B)
        }
    
        RunOp_safe(semid, V_mutex, 1);           // V(MUTEX)
        details_installed++;
    }
  
    printf("Wrench worker %d finished\n", num);
}

void screwdriver_worker(int semid, int num) {
    struct sembuf P_B[] = {{1, -1, 0}};        // P(B)
    struct sembuf P_mutex[] = {{2, -1, 0}};    // P(MUTEX)
    struct sembuf V_mutex[] = {{2, 1, 0}};     // V(MUTEX)
    struct sembuf V_A[] = {{0, 1, 0}};         // V(A)
    struct sembuf V_B[] = {{1, 1, 0}};         // V(B)
  
    int details_installed = 0;
  
        while (details_installed < devices) {
            
            RunOp_safe(semid, P_B, 1);               // P(B)
    

            RunOp_safe(semid, P_mutex, 1);           // P(MUTEX)
            
            
            printf("Screwdriver worker %d: installing screw\n", num);
            sleep(1); 
    
            
            *Ready += 1;
    
            
            if (*Ready + 1 == 3) {
                
                printf("Screwdriver worker %d: device completed\n", num);
                replace_device(semid);
      
                RunOp_safe(semid, V_A, 1);             // V(A)
                RunOp_safe(semid, V_A, 1);             // V(A)
                RunOp_safe(semid, V_B, 1);             // V(B)
            }
    
            RunOp_safe(semid, V_mutex, 1);           // V(MUTEX)
            details_installed++;
    }
  
    printf("Screwdriver worker %d finished\n", num);
}

void replace_device() {
    *Ready = 0;
    printf("-------Device replaced-------\n");
}