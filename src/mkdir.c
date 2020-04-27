#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void help(int exitStatus) {
    printf("Usage: ./mkdir DIRECTORY...\n");
    printf("Create the DIRECTORY(ies), if they do not already exist.\n");
    _exit(exitStatus);
}

int main(int argc, char *argv[]) {
    if (argc == 1)
        help(1);

    if (argc == 2 && (strcmp(argv[1], "--help") == 0))
        help(0);

    int r;

    for (int i = 1; i < argc; i++) {
        if (access(argv[i], F_OK) != -1) {
            fprintf(stderr, "The directory '%s' already exists.\n", argv[i]);
            _exit(1);
        }

        r = mkdir(argv[i], 0777);
        if (r) {
            fprintf(stderr, "Unable to create the directory %s.\n", argv[i]);
            _exit(1);
        }
    }

    _exit(0);
}
