#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>


typedef enum {
    
    SUCCESS         = 0,
    FLAG_COLLISION  = 1,
    UNKNOWN_FLAG    = 2,

}Errorcode;

int opt = 0;
int recursive_flag = 0;
int long_flag = 0;
int all_flag = 0;


struct option long_option[] = {
    {"recursive", no_argument, 0, 'R'},
    {"long", no_argument, 0, 'l'},
    {"all", no_argument, 0, 'a'},
    //lines to add flags
    {0, 0, 0, 0}
};


Errorcode flag_scan(int argc, char** argv);

int opendir_safe(DIR** d, const char* arg);


int main(int argc, char* argv[]) {
    
    if (flag_scan(argc, argv) != SUCCESS) {
        fprintf(stderr, "Flag error encountered! \nFlags for use (one at a time): -f/--force; -v/--verbose; -i/--interactive\n");
        return EXIT_FAILURE;
    }


    if ((argc == 1) || ((argc == 2) && (opt))) {
        
        DIR* d;
        opendir_safe(&d, ".");

        struct dirent* entry;
        while ((entry = readdir(d)) != NULL) {
            
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printf("%s ", entry->d_name);
            }
            
        }
        printf("\n");

        closedir(d);

    } else {
        int query;
        if(!opt) {
            query = 1;
        } else {
            query = 2;
        }

        for ( ; query < argc; query++) {

            DIR* d;
            opendir_safe(&d, argv[query]);
            
            struct dirent* entry;
            while ((entry = readdir(d)) != NULL) {
            
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    printf("%s ", entry->d_name);
                }
            
            }

            printf("\n");

            closedir(d);

        }

    }





    return SUCCESS;
}


Errorcode flag_scan(int argc, char** argv) {
    
    int flag_counter = 0;
    while ((opt = getopt_long(argc, argv, "Rla", long_option, NULL)) != -1) {
        
        switch (opt) {
            case 'R':
                recursive_flag = 1;
                flag_counter++;
                break;
            case 'l':
                long_flag = 1;
                flag_counter++;
                break;
            case 'f':
                all_flag = 1;
                flag_counter++;
                break;
            case '?':
                opt = -1;
                return UNKNOWN_FLAG;
            default:
                break;
        }

        if (flag_counter > 1) return FLAG_COLLISION; //I pb later will add combo's (e.g -a -R) 
        
    }
    
    if (flag_counter == 0) {
        opt = 1;
        return SUCCESS;
    }
    
    if (recursive_flag == 1) opt = 'R';
    if (long_flag == 1) opt = 'l';
    if (all_flag == 1) opt = 'a';
    return SUCCESS;

}


int opendir_safe(DIR** d, const char* arg) {
    *d = opendir(arg);
    if (*d == NULL) {
        fprintf(stderr,"Error opening '%s': %s\n", arg, strerror(errno));
        exit(1);
    }
    return SUCCESS;
}