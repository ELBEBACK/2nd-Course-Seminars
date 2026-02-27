#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>

int main (int argc, char *argv[]) {

    if (argc <= 1) return EXIT_SUCCESS;
    struct timeval current_time;

    pid_t pidc = fork(); 

    gettimeofday(&current_time, NULL);
    if (pidc) printf("Timestamp #1: %ld.%06ld\n", current_time.tv_sec, current_time.tv_usec);

    suseconds_t t1 = current_time.tv_sec * 1000000 + current_time.tv_usec;

    if (!pidc) {

        execvp(argv[1], &argv[1]);

        return 0;
    }

    

    

    wait(NULL);
    gettimeofday(&current_time, NULL);

    printf("Timestamp #2: %ld.%06ld\n", current_time.tv_sec, current_time.tv_usec);
    suseconds_t t2 = current_time.tv_sec * 1000000 + current_time.tv_usec;

    printf("Execution time: %ld microseconds!\n", t2 - t1);
    return 0;
}