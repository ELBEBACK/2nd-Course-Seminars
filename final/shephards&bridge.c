#include "safe-semaphors.h"
#include <sys/wait.h>
#include <stdbool.h>

struct sembuf P_HerdAB[] = {{0, -1, 0}};        // P(AB)
struct sembuf V_HerdAB[] = {{0, 1, 0}};         // V(AB)

struct sembuf P_HerdBA[] = {{1, -1, 0}};        // P(BA)
struct sembuf V_HerdBA[] = {{1, 1, 0}};         // V(BA)

struct sembuf P_Hat[]    = {{2, -1, 0}};           // P(HAT)
struct sembuf V_Hat[]    = {{2, 1, 0}};            // V(HAT)


void create_shephards(int semid);
void shephard_in_direction(int semid, int num);
void shephard_against_direction(int semid, int num);


int main(void) {
    
    int semid = create_semaphore("main.c", IPC_CREAT|0644);
    

    create_shephards(semid);

    for (size_t i = 0; i < 2; i++) wait(NULL);

    printf("All processes are ended\n");
    delete_semaphore(semid);
    return 0;
}


void create_shephards(int semid) {

    SetVal_safe(semid, 0, 1);
    SetVal_safe(semid, 1, 1);
    SetVal_safe(semid, 2, 1);

    pid_t pid = fork_safe();
    if (pid == 0) {
        shephard_in_direction(semid, 1);
        exit(EXIT_SUCCESS);
    }

    pid = fork_safe();
    if (pid == 0) {
        shephard_against_direction(semid, 1);
        exit(EXIT_SUCCESS);
    }

}


void shephard_in_direction(int semid, int num) {
    
    printf("%d\n", GetVal_safe(semid, 0));
    printf("Shephard A came to the end A with his herd\n");
    printf("Shephard A is on his way to the end B\n");
    sleep(1);
    printf("Shephard A went through the bridge to the end B alone and left the hat there\n");
    RunOp_safe(semid, P_HerdBA, 1);                 // P(MUTEX_B)

    printf("Shephard A to B is on his way to the end A\n");
    sleep(1);
    printf("Shephard A went back to the end A to take his herd\n");
    printf("Shephard A checks if there is anyone crossing against his direction\n");
    while (!GetVal_safe(semid, 0)) {
        printf("Shephard A waits...\n");
        sleep(1);
    }

    printf("Shephard A is on his way to the end B\n");
    sleep(3);
    printf("Shephard A came with his herd to the end B and took the hat\n");
    RunOp_safe(semid, V_HerdBA, 1);                 // V(MUTEX_B)

    printf("Shephard 0 finished crossing!\n");

}


void shephard_against_direction(int semid, int num) {

    printf("Shephard B came to the end B with his herd\n");
    printf("Shephard B checks if there is any hat on his end\n");
    while (!GetVal_safe(semid, 1)) {                  
        printf("Shephard B waits...\n");
        sleep(1);
    }
    RunOp_safe(semid, P_HerdAB, 1);                 // P(MUTEX_A)
    printf("%d\n", GetVal_safe(semid, 0));
    printf("%d\n", GetVal_safe(semid, 1));
    printf("Shephard B is on his way to the end A\n");
    sleep(3);
    printf("Shephard B crossed the bridge with his herd to the end A\n");
    RunOp_safe(semid, P_HerdAB, 1);                 // V(MUTEX_B)

    printf("Shephard 1 finished crossing!\n");
}