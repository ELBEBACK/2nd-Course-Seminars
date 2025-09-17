#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

#define PAGE_SIZE 4096

typedef enum {
    
    SUCCESS         = 0,
    FLAG_COLLISION  = 1,
    UNKNOWN_FLAG    = 2,
    FILE_ABSCENCE   = 3,

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

Errorcode flag_scan(int argc, char** argv, int* opt, int* longindex);
Errorcode fdcopy(int argc, char** argv, int* opt, int* longindex);


int main(int argc, char* argv[]) {
    int longindex = -1;
    if (flag_scan(argc, argv, &opt, &longindex) != SUCCESS) {
        fprintf(stderr, "FLAG ERROR ENCOUNTERED\n");
        return EXIT_FAILURE;
    }





    return EXIT_SUCCESS;
}


Errorcode flag_scan(int argc, char** argv, int* opt, int* longindex) {
    
    int flag_counter = 0;
    while ((*opt = getopt_long(argc, argv, "vif", long_option, NULL)) != -1) {
        
        *longindex = optind - 1;
        switch (*opt) {
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
                *opt = -1;
                return UNKNOWN_FLAG;
            default:
                break;
        }

        if (flag_counter > 1) return FLAG_COLLISION;
        
    }
    
    if (flag_counter == 0) {
        *opt = 0;
        return SUCCESS;
    }
    
    if (verbose_flag == 1) *opt = 'v';
    if (interactive_flag == 1) *opt = 'i';
    if (force_flag == 1) *opt = 'f';
    return SUCCESS;

}


Errorcode fdcopy(int argc, char** argv, int* opt, int* longindex) {
    return 0;
}