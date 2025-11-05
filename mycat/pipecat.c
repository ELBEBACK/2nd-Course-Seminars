#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PAGE_SIZE 4096


int fd_copy(int fd_src, int fd_dst);
int fd_cat(int* pipefd, int fd_src);


int main(int argc, const char** argv) {

    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Error setting a pipe: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    if (argc == 1) {
        //STDIN -> pipe -> STDOUT 
        fd_cat(pipefd, 0);
    
    } else {

        int fd = 0;
        for (int c = 1; c < argc; c++) {
            fd = open(argv[c], O_RDONLY);
            if (fd == -1) { 
                fprintf(stderr,"Error opening file '%s': %s\n", argv[c], strerror(errno));
                return EXIT_FAILURE;
            } else { 
                fd_cat(pipefd, fd);
                close(fd);
            }
            
        }
    }
    
    return EXIT_SUCCESS;
}


int fd_copy(int fd_src, int fd_dst) {
        
    char buff[PAGE_SIZE];
        
    ssize_t rnum = 0;

    do { 
        
        rnum = read(fd_src, buff, PAGE_SIZE);
        if (rnum == 0) break;

        ssize_t wnum = 0;
        char* buff_p = buff;
        while (rnum != 0) {

            buff_p += wnum;
            rnum -= wnum;

            wnum = write(fd_dst, buff_p, rnum);
            if (wnum < 0) {
                perror("write()\n");
                return 1;
            }
        }

    } while (1);

    return 0;
}


int fd_cat(int* pipefd, int fd_src) {
    
    pid_t child = fork();

    if (!child) {
        close(pipefd[0]);
        fd_copy(fd_src, pipefd[1]);
        close(pipefd[1]);
        return 0;
    }

    close(pipefd[1]);
    fd_copy(pipefd[0], 1);
    close(pipefd[0]);

    wait(NULL);

    return 0;
}