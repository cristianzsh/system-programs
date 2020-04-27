#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

void help(int exitStatus) {
    printf("Usage: mv SOURCE DEST\n");
    printf("  or:  mv SOURCE... DIRECTORY\n");
    printf("Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY.\n");
    _exit(exitStatus);
}

char *fullpath(char *str1, char *str2) {
    size_t bufflen = strlen(str1) + strlen(str2) + 2;
    char *dest = malloc(bufflen * sizeof(char));

    if (dest) {
        strcpy(dest, str1);
        strcat(dest, "/");
        strcat(dest, str2);
    }

    return dest;
}

int main(int argc, char *argv[]) {
    if (argc == 1)
        help(1);

    if (argc == 2 && (strcmp(argv[1], "--help") == 0))
        help(0);

    int moveToDir = 0, r = 0;
    char *newpath;
    struct stat st;
    stat(argv[argc - 1], &st);

    if (!S_ISDIR(st.st_mode) && argc > 3)
        help(1);
    else if (S_ISDIR(st.st_mode))
        moveToDir = 1;

    if (moveToDir) {
        for (int i = 1; i < argc - 1; i++) {
            newpath = fullpath(argv[argc - 1], argv[i]);
            r = rename(argv[i], newpath);
            free(newpath);
            if (r != 0) {
                fprintf(stderr, "Unable to move the file %s.\n", argv[i]);
                _exit(1);
            }
        }
    } else {
        r = rename(argv[1], argv[argc - 1]);
        if (r != 0) {
            fprintf(stderr, "Unable to move the file %s to %s.\n", argv[1], argv[2]);
            _exit(1);
        }
    }

    _exit(0);
}
