#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


#define MAX_LINEBUFSIZE     (256)
#define MAX_ARGV            (8)
#define MAX_ARGSIZE         (24)
#define MAX_COMMAND         (8)

typedef struct{
    char arg_line[MAX_ARGV][MAX_ARGSIZE];
    int argv_num;
}command;


int commandl_parser(char* commandl_src, command* commands_out, int* command_num);
int commandl_exec(command* commands, int command_num);
void commandl_dump(command* commands, int command_num);

int main(const int argc, const char** argv) {

    char linebuf[MAX_LINEBUFSIZE] = {0};
    command command_lst[MAX_COMMAND] = {0};
    int command_num = 0;

    while(1) {

        printf("$ ");
        
        if(!fgets(linebuf, sizeof(linebuf), stdin)) {
            fprintf(stderr, "\n\033[1m%s\033[0m: exit \n", argv[0]);
            return EXIT_SUCCESS;
        }

        
        memset(command_lst, 0, sizeof(command) * MAX_COMMAND);
        if(commandl_parser(linebuf, command_lst, &command_num)) {
            fprintf(stderr, "\n\033[1m%s\033[0m: Error related to input format occurred! \n", argv[0]);
            return EXIT_FAILURE;
        }

        commandl_dump(command_lst, command_num);
        commandl_exec(command_lst, command_num);

    }

    return EXIT_SUCCESS;
}


int commandl_parser(char* commandl_src, command* commands_out, int* command_num) {

    char* buffer = (char*) calloc(MAX_LINEBUFSIZE, sizeof(char));
    if(!buffer) return EXIT_FAILURE;

    strcpy(buffer, commandl_src);

    buffer[strcspn(buffer, "\n")] = '\0';
    int buffsize = strlen(buffer);
    
    for(int c = 0; c < buffsize ; c++) {
        if (*(buffer + c) == ' ') {
            *(buffer + c) = '\0';
        }
    }

    char* position = buffer;
    int token_size = 0;
    int commands = 1, argv_num = 0;

    for( position = buffer; (int) (position - buffer) < buffsize ; ) {
        
        while (*position == '\0') {
            
            position++;

            if(*position == '|') {
                commands_out[commands - 1].argv_num = argv_num;
                position++, commands++;
                argv_num = 0;
                
            }
        }

        int current = (int) (position - buffer);
        if(current >= buffsize) break;

        token_size = strlen(position);
        if(token_size > MAX_ARGSIZE) {
            free(buffer);
            return EXIT_FAILURE; //wrong format, arg too long
        }

        strcpy(commands_out[commands - 1].arg_line[argv_num], position);
        position += token_size;
        argv_num++;
    }
    commands_out[commands - 1].argv_num = argv_num;
    *command_num = commands;
    free(buffer);
    return EXIT_SUCCESS;
}


int commandl_exec(command* commands, int command_num) {
    
    int pipefd[7][2];
    char* execv_argv[MAX_ARGV] = {NULL};

    for (int i = 0; i < command_num; i++) {
        
        if (pipe(pipefd[i]) == -1) {
            fprintf(stderr, "Error setting a pipe: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        pid_t childp = fork();
        
        if (!childp) {    
            if (i == 0 && i != command_num - 1) {

                close(pipefd[i][0]);
                dup2(pipefd[i][1], STDOUT_FILENO);
            
            } else if (i == command_num - 1) {

                close(pipefd[i][1]);
                dup2(pipefd[i][0], STDIN_FILENO);

            } else {

                dup2(pipefd[i][0], STDIN_FILENO);
                dup2(pipefd[i][1], STDOUT_FILENO);

            } 
            
            memset(execv_argv, 0, sizeof(execv_argv));
            for(int c = 0; c < commands[i].argv_num; c++) {
                if (commands[i].arg_line[c][0] != '\0') {
                    execv_argv[c] = commands[i].arg_line[c];
                } else {
                    break;
                }

            }

            execvp(execv_argv[0], execv_argv + 1);
            perror(execv_argv[0]);
            return EXIT_SUCCESS;
        }

    }

    for (int i = 0; i < command_num; i++) wait(NULL);

    return EXIT_SUCCESS;

}

void commandl_dump(command* commands, int command_num) {
    
    for(int i = 0; i < 8 ; i++) {
        
        for(int j = 0; j < 8 ; j++) {
            
            if (*(commands[i].arg_line[j]) != '\0') {
                printf("command[%d] argv[%d]: %s\n", i, j, commands[i].arg_line[j]);
            } else {
                continue;
            }
        
        }
        if (*(commands[i].arg_line[0]) != '\0') {
            printf("Number of arguments: %d\n", commands[i].argv_num);        
        }
    }

    printf("Number of commands: %d\n", command_num);
}