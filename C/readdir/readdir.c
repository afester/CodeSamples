#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main (int argc, char *argv[])
{
    struct dirent *dirent;
    struct stat filestat;
    DIR* dirp;

    if (argc != 2) {
        printf("error");
        return 1;
    }

    if ((dirp = opendir(argv[1])) == NULL) {
        printf("error");
        return 1;
    }

    while ((dirent = readdir(dirp)) != NULL)
    {
        stat(dirent->d_name, &filestat);
        printf("%s %d\n", dirent->d_name, dirent->d_type);    
    }

    return 0;
}
