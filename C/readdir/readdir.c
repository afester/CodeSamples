#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main (int argc, char *argv[]) {
    struct dirent *dirent;
    DIR* dirp;
    struct stat filestat;

    if (argc != 2) {
        printf("Usage: %s pathName\n\n", argv[0]);
        return 1;
    }

    if ((dirp = opendir(argv[1])) == NULL) {
        perror("opendir");
        return 1;
    }

    while ((dirent = readdir(dirp)) != NULL) {
        char filename[1024];  // TODO!
        strcpy(filename, argv[1]);
        strcat(filename, "/");
        strcat(filename, dirent->d_name);
        if (stat(filename, &filestat) == -1) {
           perror("stat");
        }
        printf("%-6d %-6zu %s\n", dirent->d_type, filestat.st_size, dirent->d_name);
    }

    return 0;
}
