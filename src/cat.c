#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void help(int exitStatus) {
    printf("Usage: ./cat [FILE]...\n");
    printf("Show the contents of the FILEs.\n");
    _exit(exitStatus);
}

int main(int argc, char *argv[]) {
    if (argc == 1)
        help(1);

    if (argc == 2 && (strcmp(argv[1], "--help") == 0))
        help(0);

    int n, r, f;
    char c[1024];
    struct stat st;

    for (int i = 1; i < argc; i++) {
        r = stat(argv[i], &st);

        if (S_ISDIR(st.st_mode)) {
            fprintf(stderr, "cat: %s: Is a directory\n", argv[i]);
            help(1);
        }

        if (r < 0) {
            fprintf(stderr, "Error when trying to read file %s\n", argv[i]);
            help(1);
        }

        f = open(argv[i], O_RDONLY);
        while ((n = read(f, c, 1024)) > 0)
            write(1, c, n);
    }

    _exit(0);
}
