#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

void help(int exitStatus) {
    printf("Usage: cp SOURCE DEST\n");
    printf("  or:  cp SOURCE... DIRECTORY\n");
    _exit(exitStatus);
}

void fatal(char *fp) {
    fprintf(stderr, "Unable to read the file %s.\n", fp);
    _exit(1);
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

int getPermissions(char *fn) {
    struct stat st;

    if (stat(fn, &st) == -1 || S_ISDIR(st.st_mode))
        return -1;

    return (st.st_mode & S_IRUSR) | (st.st_mode & S_IWUSR) | (st.st_mode & S_IXUSR) |
        (st.st_mode & S_IRGRP) | (st.st_mode & S_IWGRP) | (st.st_mode & S_IXGRP) |
        (st.st_mode & S_IROTH) | (st.st_mode & S_IWOTH)|(st.st_mode & S_IXOTH);
}

void copy(char *sourcefn, char *targetfn) {
    int source = open(sourcefn, O_RDONLY);
    int target = open(targetfn, O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR);

    if (target == -1)
        fatal(targetfn);

    int s;
    char buf[1024];

    while((s = read(source, buf, 1024)) != 0)
        write(target, buf, s);

    close(source);
    close(target);
}


int main(int argc, char *argv[]) {
    if (argc == 1)
        help(1);

    if (argc == 2 && (strcmp(argv[1], "--help") == 0))
        help(0);

    int copyToDir = 0;
    struct stat st;
    stat(argv[argc - 1], &st);

    if (!S_ISDIR(st.st_mode) && argc > 3)
        help(1);
    else if (S_ISDIR(st.st_mode))
        copyToDir = 1;

    int p = 0;
    char *fp;
    if (copyToDir) {
        for (int i = 1; i < argc - 1; i++) {
            p = getPermissions(argv[i]);
            if (p == -1)
                fatal(argv[i]);

            fp = fullpath(argv[argc - 1], argv[i]);
            copy(argv[1], fp);
            chmod(fp, p);
            free(fp);
        }
    } else {
        p = getPermissions(argv[1]);
        if (p == -1)
            fatal(argv[1]);

        copy(argv[1], argv[2]);
        chmod(argv[2], p);
    }

    _exit(0);
}
