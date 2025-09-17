#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define PAGE_SIZE 4096


int output(int fdstream);


int main(int argc, char* argv[]) {
        
    if (argc == 1) {
    
        output(0);
    
    } else {

        int fd = 0;
        for (int c = 1; c < argc; c++) {
            fd = open(argv[c], O_RDONLY);
            if (fd == -1) { 
                fprintf(stderr,"Error opening file '%s': %s\n", argv[c], strerror(errno));
            } else { 
                output(fd);
                close(fd);
            }
            
        }
    }
    
    return 0;
}


int output(int fdstream) {
        
    char buff[PAGE_SIZE];
        
    ssize_t rnum = 0;

    do { 
        
        rnum = read(fdstream, buff, PAGE_SIZE);
        if (rnum == 0) break;

        ssize_t wnum = 0;
        char* buff_p = buff;
        while (rnum != 0) {

            buff_p += wnum;
            rnum -= wnum;

            wnum = write(1, buff_p, rnum);
                
        }

    } while (1);
}