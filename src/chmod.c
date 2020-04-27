#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void help(int exitStatus) {
    printf("Usage: ./chmod OCTAL-MODE FILE...\n");
    printf("Change the mode of each FILE to OCTAL-MODE.\n");
    _exit(exitStatus);
}

int main(int argc, char *argv[]) {
    if (argc < 3)
        help(1);

    int p = strtol(argv[1], 0, 8);

    for (int i = 2; i < argc; i++) {
        if (chmod(argv[i], p) < 0) {
            fprintf(stderr, "Error when changing file permissions for %s!\n", argv[i]);
            _exit(1);
        }
    }

    _exit(0);
}
