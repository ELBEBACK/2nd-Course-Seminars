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
#include <time.h>

typedef enum {

    SUCCESS         = 0,
    FLAG_COLLISION  = 1,
    UNKNOWN_FLAG    = 2,

} Errorcode;

int opt = 0;
int recursive_flag = 0;
int long_flag = 0;
int all_flag = 0;


struct option long_option[] = {
    {"recursive", no_argument, 0, 'R'},
    {"long", no_argument, 0, 'l'},
    {"all", no_argument, 0, 'a'},
    {0, 0, 0, 0}
};


void print_file_details(const char *path, struct stat *statbuf);
void list_directory_recursive(const char *dirpath);


int main(int argc, char *argv[]) {
    int opt_index = 0;
    while ((opt = getopt_long(argc, argv, "Rla", long_option, &opt_index)) != -1) {
        switch (opt) {
            case 'R':
                recursive_flag = 1;
                break;
            case 'l':
                long_flag = 1;
                break;
            case 'a':
                all_flag = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-R] [-l] [-a] <directory>\n", argv[0]);
                exit(UNKNOWN_FLAG);
        }
    }

    if (optind < argc) {
        list_directory_recursive(argv[optind]);
    } else {
        fprintf(stderr, "Flag error encountered! \nFlags for use (one at a time): -f/--force; -v/--verbose; -i/--interactive\n");
        exit(UNKNOWN_FLAG);
    }

    return SUCCESS;
}


void print_file_details(const char *path, struct stat *statbuf) {
    if (long_flag) {
        char perms[10];
        snprintf(perms, sizeof(perms), "%c%c%c%c%c%c%c%c%c",
                 (statbuf->st_mode & S_IRUSR) ? 'r' : '-',
                 (statbuf->st_mode & S_IWUSR) ? 'w' : '-',
                 (statbuf->st_mode & S_IXUSR) ? 'x' : '-',
                 (statbuf->st_mode & S_IRGRP) ? 'r' : '-',
                 (statbuf->st_mode & S_IWGRP) ? 'w' : '-',
                 (statbuf->st_mode & S_IXGRP) ? 'x' : '-',
                 (statbuf->st_mode & S_IROTH) ? 'r' : '-',
                 (statbuf->st_mode & S_IWOTH) ? 'w' : '-',
                 (statbuf->st_mode & S_IXOTH) ? 'x' : '-');

        char time_str[100];
        struct tm *time_info = localtime(&statbuf->st_mtime);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);

        printf("%s %ld %s %s\n", perms, statbuf->st_size, time_str, path);
    } else {
        printf("%s\n", path);
    }
}


void list_directory_recursive(const char *dirpath) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[1024];

    dir = opendir(dirpath);
    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {

        if (!all_flag && entry->d_name[0] == '.') {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);

        if (lstat(path, &statbuf) == -1) {
            perror("lstat");
            continue;
        }

        print_file_details(path, &statbuf);

        if (recursive_flag && S_ISDIR(statbuf.st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            
            printf("\n%s:\n", path);
            list_directory_recursive(path); 
        }
    }

    closedir(dir);
}