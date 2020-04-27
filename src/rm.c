#define _XOPEN_SOURCE 500

#include <dirent.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void help(int exitStatus) {
    printf("Usage: ./rm [OPTION]... [FILE]...\n");
    printf("Remove (unlink) the FILE(s).\n\n");
    printf("  -r   remove directories and their contents recursively\n");
    _exit(exitStatus);
}

int isDir(const char *path) {
    struct stat s;

    if (lstat(path, &s) == -1)
        return 0;

    return S_ISDIR(s.st_mode);
}

int unlinkDir(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    int r = remove(fpath);
    if (r)
        perror(fpath);

    return r;
}

int rmrf(char *path) {
    return nftw(path, unlinkDir, 64, FTW_DEPTH | FTW_PHYS);
}

int main(int argc, char *argv[]) {
    if (argc == 1)
        help(1);

    if (argc >= 2 && (strcmp(argv[1], "--help") == 0))
        help(0);

    int r = 0, recursive = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            recursive = 1;
            continue;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0)
            continue;

        if (isDir(argv[i]) && (strcmp(argv[i], "-r") != 0)) {
            if (!recursive) {
                fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", argv[i]);
                _exit(1);
            }

            rmrf(argv[i]);
        } else {
            r = unlink(argv[i]);
        }

        if (r == -1) {
            if (access(argv[1], F_OK) != -1)
                fprintf(stderr, "rm: cannot remove '%s': Permission denied\n", argv[i]);
            else
                fprintf(stderr, "rm: cannot remove '%s': No such file or directory\n", argv[i]);

            _exit(1);
        }
    }

    _exit(0);
}
