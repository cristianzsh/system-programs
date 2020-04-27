#include <ctype.h>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void getCmdline(char *path) {
    strcat(path, "/cmdline");
    FILE *file = fopen(path, "r");
    char cmd[1024];
    int c;

    while ((c = getc(file)) != EOF) {
        if (c == 0)
            printf(" ");
        else
            printf("\033[1;32m%c", c);
    }

    fclose(file);
    printf("\033[0m\n");
}

char *getDirUser(char *dir) {
    struct passwd *pw;
    struct stat st;

    stat(dir, &st);

    pw = getpwuid(st.st_uid);
    return pw->pw_name;
}

int main(int argc, char *argv[]) {
    DIR *proc;
    struct dirent *ep;
    proc = opendir("/proc/");
    if (proc == NULL) {
        fprintf(stderr, "Could not open /proc!\n");
        _exit(1);
    }

    char dir[100];
    printf("USER\tPID\tCMD\n");

    while (ep = readdir(proc)) {
        if (!isdigit(ep->d_name[0]))
            continue;

        dir[0] = '\0';
        strcpy(dir, "/proc/");
        strcat(dir, ep->d_name);

        printf("%s\t%s\t", getDirUser(dir), ep->d_name);
        getCmdline(dir);
    }

    closedir(proc);
    _exit(0);
}
