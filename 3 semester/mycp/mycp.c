#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/stat.h>

#define PAGE_SIZE 4096

#define OPEN(file, fd, mode) if((fd = open(file, mode, 0666)) == -1) {fprintf(stderr,"Error opening file '%s': %s\n", file, strerror(errno)); if (errno = EEXIST) {if(S_ISDIR(argv_elem.st_mode)) {free(fd_src); free(fd_dst);}} return INACCESSIBLE_FILE;}


typedef enum {
    
    SUCCESS             = 0,
    FLAG_COLLISION      = 1,
    UNKNOWN_FLAG        = 2,
    INACCESSIBLE_FILE   = 3,
    FORMAT_ERROR        = 4,

}Errorcode;


int opt;
int verbose_flag = 0;
int interactive_flag = 0;
int force_flag = 0;


struct option long_option[] = {
    {"verbose", no_argument, 0, 'v'},
    {"interactive", no_argument, 0, 'i'},
    {"force", no_argument, 0, 'f'},
    {0, 0, 0, 0}
};

Errorcode flag_scan(int argc, char** argv);
Errorcode fdcopy(int argc, char** argv);

void fd2fd_copy(int fd_src, int fd_dst);


int main(int argc, char* argv[]) {
    
    if (flag_scan(argc, argv) != SUCCESS) {
        fprintf(stderr, "Flag error encountered! \nFlags for use (one at a time): -f/--force; -v/--verbose; -i/--interactive\n");
        return EXIT_FAILURE;
    }
    
    switch (fdcopy(argc, argv)) {
        
        case SUCCESS:
            break; 

        case FORMAT_ERROR: 
            fprintf(stderr, "Error copying file(s)! \nUsage: cp [OPTION] SOURCE DEST \n   or: cp [OPTION] SOURCE... DIRECTORY\n");
            return EXIT_FAILURE;

        default: 
            return EXIT_FAILURE;
    }
    

    return EXIT_SUCCESS;
}


Errorcode flag_scan(int argc, char** argv) {
    
    int flag_counter = 0;
    while ((opt = getopt_long(argc, argv, "vif", long_option, NULL)) != -1) {
        
        switch (opt) {
            case 'v':
                verbose_flag = 1;
                flag_counter++;
                break;
            case 'i':
                interactive_flag = 1;
                flag_counter++;
                break;
            case 'f':
                force_flag = 1;
                flag_counter++;
                break;
            case '?':
                opt = -1;
                return UNKNOWN_FLAG;
            default:
                break;
        }

        if (flag_counter > 1) return FLAG_COLLISION;
        
    }
    
    if (flag_counter == 0) {
        opt = 1;
        return SUCCESS;
    }
    
    if (verbose_flag == 1) opt = 'v';
    if (interactive_flag == 1) opt = 'i';
    if (force_flag == 1) opt = 'f';
    return SUCCESS;

}


Errorcode fdcopy(int argc, char** argv) {
    
    if (opt == 'i') {
        char answer;
        printf("Do you want to proceed with the copy?(Y/n) ");
        scanf("%c", &answer);
        if (answer != 'Y' && answer != 'y') return SUCCESS;
    }

    if (argc - optind < 2) return FORMAT_ERROR;

    struct stat argv_elem;
    if (stat(argv[argc - 1], &argv_elem) == -1) {
        return FORMAT_ERROR;
    }

    
    if (S_ISDIR(argv_elem.st_mode)) {
        
        int nsrc = argc - optind;
        int* fd_src = (int*) calloc(nsrc, sizeof(int));
        int* fd_dst = (int*) calloc(nsrc, sizeof(int));

        char fullpath[256];
        for (int c = 0; c < nsrc - 1; c++) {
            
            OPEN(argv[optind + c], fd_src[c], O_RDONLY);
            
            memset(fullpath, 0, sizeof(fullpath));
    
            snprintf(fullpath, sizeof(fullpath), "%s/%s", argv[argc - 1], argv[optind + c]);
            //printf("%s\n", fullpath);
            if (opt == 'f') { 
                OPEN(fullpath, fd_dst[c], O_WRONLY | O_CREAT | O_TRUNC);
            } else {
                OPEN(fullpath, fd_dst[c], O_WRONLY | O_CREAT | O_EXCL);
                
            }
        }

        for (int c = 0; c < nsrc - 1; c++) {

            fd2fd_copy(fd_src[c], fd_dst[c]);
            
            if (opt == 'v') {
                lseek(fd_src[c], 0, SEEK_SET);
                fd2fd_copy(fd_src[c], STDOUT_FILENO); 
            }

            close(fd_dst[c]);
            close(fd_src[c]);
        }

        free(fd_src);
        free(fd_dst);
    

    } else if (S_ISREG(argv_elem.st_mode)) {
        
        if (argc - optind > 2) return FORMAT_ERROR;
        
        int fd_src, fd_dst;

        OPEN(argv[optind], fd_src, O_RDONLY);
        OPEN(argv[optind + 1], fd_dst, O_WRONLY);

        fd2fd_copy(fd_src, fd_dst);

        if (opt == 'v') {
            lseek(fd_src, 0, SEEK_SET);
            fd2fd_copy(fd_src, STDOUT_FILENO); 
        }


        close(fd_src);
        close(fd_dst);
    

    } else {
        return FORMAT_ERROR;
    }


    return SUCCESS;
}


void fd2fd_copy(int fd_src, int fd_dst) {
    
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
                perror("Error returned in write()\n");
                return;
            } 
        }

    } while (1);

}