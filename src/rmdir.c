#define _XOPEN_SOURCE 500

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void help(int exitStatus) {
    printf("Usage: ./rmdir DIRECTORY...\n");
    printf("Remove the DIRECTORY(ies), if they are empty.\n");
    _exit(exitStatus);
}

int countEntries(const char *path) {
    DIR *dir = opendir(path);
    int n = 0;
    struct dirent *d;

    if (dir == NULL)
        return 0;

    while ((d = readdir(dir)) != NULL)
        n++;

    closedir(dir);
    return n;
}

int main(int argc, char *argv[]) {
    if (argc == 1)
        help(1);

    if (argc == 2 && (strcmp(argv[1], "--help") == 0))
        help(0);

    int nf;
    struct stat st;

    for (int i = 1; i < argc; i++) {
        nf = countEntries(argv[i]);

        if (nf > 2) {
            fprintf(stderr, "rmdir: failed to remove '%s': Directory not empty -> %d file(s)\n", argv[i], nf - 2);
            _exit(1);
        }

        if (stat(argv[i], &st) != 0) {
            fprintf(stderr, "rmdir: failed to remove '%s': Directory does not exist\n", argv[i]);
            _exit(1);
        }

        if (access(argv[i], W_OK) != 0) {
            fprintf(stderr, "rmdir: failed to remove '%s': Permission denied\n", argv[i]);
            _exit(1);
        }

        rmdir(argv[i]);
    }

    _exit(0);
}
