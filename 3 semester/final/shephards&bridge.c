#include "safe-semaphores.h"
#include <sys/wait.h>
#include <stdbool.h>

int *A_QUEUE;

struct sembuf P_mutex[] = {{0, -1, 0}};    // P(MUTEX)
struct sembuf V_mutex[] = {{0, 1, 0}};     // V(MUTEX)
struct sembuf P_APASS[] = {{1, -1, 0}};    // P(APASS)
struct sembuf V_APASS[] = {{1, 1, 0}};     // V(APASS)
struct sembuf P_BPASS[] = {{2, -1, 0}};    // P(BPASS)
struct sembuf V_BPASS[] = {{2, 1, 0}};    // V(BPASS)
struct sembuf P_BRIDGE[] = {{3, -1, 0}};   // P(BRIDGE)
struct sembuf V_BRIDGE[] = {{3, 1, 0}};    // V(BRIDGE)

void create_shephards(int semid, int numA, int numB);
void shephard_A_to_B(int semid, int num);
void shephard_B_to_A(int semid, int num);


// sem_array = | BPASS = 1 | APASS = 1 | BRIDGE = 1 | mutex = 1 |
int main(const int argc, const char *argv[]) {
    
    if (argc < 3) return EXIT_SUCCESS;

    int shmid = Shmget_safe(sizeof(int));
    A_QUEUE = Shmat_safe(shmid);
    *A_QUEUE = 0;

    int semid = create_semaphore("main.c", IPC_CREAT|0644);
    
    int numA = atoi(argv[1]), numB = atoi(argv[2]);
    create_shephards(semid, numA, numB);

    for (size_t i = 0; i < numA + numB; i++) wait(NULL);

    printf("All processes are ended\n");

    shmdt(A_QUEUE);                 
    shmctl(shmid, IPC_RMID, NULL);  

    delete_semaphore(semid);
    return 0;
}


void create_shephards(int semid, int numA, int numB) {

    SetVal_safe(semid, 0, 1);
    SetVal_safe(semid, 1, 1);
    SetVal_safe(semid, 2, 1);
    SetVal_safe(semid, 3, 1);

    for (int i = 1; i <= numA; i++) {
        pid_t pid = fork_safe();
        if (pid == 0) {
        shephard_A_to_B(semid, i);
        exit(EXIT_SUCCESS);
        }
    }

    for (int i = 1; i <= numB; i++) {
        pid_t pid = fork_safe();
        if (pid == 0) {
        shephard_B_to_A(semid, i);
        exit(EXIT_SUCCESS);
        }
    }

}


void shephard_A_to_B(int semid, int num) {
    
    printf("Shephard A #%d came to the end A with his herd\n", num);
    RunOp_safe(semid, P_mutex, 1);
    *A_QUEUE += 1;
    if (*A_QUEUE == 1) {
        printf("Shephard A #%d went through the bridge to the end B alone and left the hat there\n", num);
        RunOp_safe(semid, P_BPASS, 1);
        printf("Shephard A #%d went back to the end A to take his herd\n", num);
    }
    RunOp_safe(semid, V_mutex, 1);

    printf("Shephard A #%d checks if there is anyone crossing against his direction\n", num);
    RunOp_safe(semid, P_APASS, 1);
    RunOp_safe(semid, V_APASS, 1);
    printf("Shephard A #%d is on his way to the end B\n", num);
    RunOp_safe(semid, P_BRIDGE, 1);
    printf("Shephard A #%d came with his herd to the end B\n", num); 
    RunOp_safe(semid, P_mutex, 1);
    *A_QUEUE -= 1;
    if (*A_QUEUE == 0) {
        printf("Shephard A #%d took the hat\n", num);
        RunOp_safe(semid, V_BPASS, 1);
    }
    RunOp_safe(semid, V_mutex, 1);
    RunOp_safe(semid, V_BRIDGE, 1);

    printf("Shephard A #%d finished crossing!\n", num);

}


void shephard_B_to_A(int semid, int num) {

    printf("Shephard B #%d came to the end B with his herd\n", num);
    printf("Shephard B #%d checks if there is any hat on his end\n", num);
    RunOp_safe(semid, P_BPASS, 1);
    RunOp_safe(semid, P_APASS, 1);
    printf("Shephard B #%d is on his way to the end A\n", num);
    RunOp_safe(semid, P_BRIDGE, 1);
    RunOp_safe(semid, V_BPASS, 1);
    printf("Shephard B #%d crossed the bridge with his herd to the end A\n", num);
    RunOp_safe(semid, V_BRIDGE, 1);
    RunOp_safe(semid, V_APASS, 1);

    printf("Shephard B #%d finished crossing!\n", num);

}