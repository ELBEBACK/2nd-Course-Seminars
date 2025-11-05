#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define PAGE_SIZE 1024

typedef struct {
    int bytes;
    int words;
    int lines;
} counter_info;

int fd_copy_wcounter(int fd_src, int fd_dst, counter_info* reg);
int pseudo_word_counter(char* buf, int size);
int line_counter(char* buf, int size);


int main (int argc, char *argv[]) {

    if (argc <= 1) return EXIT_SUCCESS;

    
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Error setting a pipe: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    
    pid_t pidc = fork(); 

    if (!pidc) {
        close(pipefd[0]);

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        execvp(argv[1], &argv[1]);

        return 0;
    }

    wait(NULL);

    close(pipefd[1]);
    
    counter_info info = {0, 0, 0};
    fd_copy_wcounter(pipefd[0], STDOUT_FILENO, &info);
    
    close(pipefd[0]);

    printf("\nThe number of bytes: %d\nThe number of words: %d\nThe number of lines: %d\n", info.bytes, info.words, info.lines);

    return 0;
}


int fd_copy_wcounter(int fd_src, int fd_dst, counter_info* reg) {
        
    char buff[PAGE_SIZE];
        
    ssize_t rnum = 0;

    do { 
        
        rnum = read(fd_src, buff, PAGE_SIZE);
        if (rnum == 0) break;

        reg->bytes += rnum;
        reg->words += pseudo_word_counter(buff, rnum);
        reg->lines += line_counter(buff, rnum);

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


int pseudo_word_counter(char* buf, int size) {
    int nword = 0;

    for (int c = 0; c < size; c++) {
        if ((buf[c] == ' ') || (buf[c] == '\n')) nword++; 
    }

    return nword;
}


int line_counter(char* buf, int size) {
    int nline = 0;

    for (int c = 0; c < size; c++) {
        if (buf[c] == '\n') nline++; 
    }

    return nline;
}