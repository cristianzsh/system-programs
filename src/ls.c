#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>

struct linuxDirent {
    long dIno;
    off_t dOff;
    unsigned short dReclen;
    char dName[];
};

void help(int exitStatus) {
    printf("Usage: ./ls [FILE]\n");
    printf("List information about the FILE (the current directory by default).\n");
    _exit(exitStatus);
}

int main(int argc, char *argv[]) {
    if (argc == 2 && (strcmp(argv[1], "--help") == 0))
        help(0);

    int fd, nread, bpos;
    char buff[1024], dType, cwd[PATH_MAX];
    char *lspath = argc > 1 ? argv[1] : cwd;
    struct linuxDirent *d;
    struct stat fileStat;
    struct passwd *pw;
    struct group *gr;
    getcwd(cwd, sizeof(cwd));

    fd = open(lspath, O_RDONLY | O_DIRECTORY);
    chdir(lspath);
    stat(lspath, &fileStat);

    switch (fileStat.st_mode & S_IFMT) {
        case S_IFBLK:
            printf("%s is a lock device.\n", lspath);
            _exit(0);
        case S_IFCHR:
            printf("%s is a character device.\n", lspath);
            _exit(0);
        case S_IFIFO:
            printf("%s is a FIFO/pipe.\n", lspath);
            _exit(0);
        case S_IFLNK:
            printf("%s is a symlink.\n", lspath);
            _exit(0);
        case S_IFREG:
            printf("%s is a regular file.\n", lspath);
            _exit(0);
        case S_IFSOCK:
            printf("%s is a socket.\n", lspath);
            _exit(0);
    }

    if (access(lspath, R_OK) != 0) {
        fprintf(stderr, "Error when trying to read the directory %s.\n", lspath);
        _exit(1);
    }

    for (;;) {
        nread = syscall(SYS_getdents, fd, buff, 1024);
        if (nread == 0)
            break;

        for (bpos = 0; bpos < nread; ) {
            d = (struct linuxDirent*) (buff + bpos);
            stat(d->dName, &fileStat);
            dType = *(buff + bpos + d->dReclen - 1);

            switch (fileStat.st_mode & S_IFMT) {
                case S_IFBLK:  printf("b"); break;
                case S_IFCHR:  printf("c"); break;
                case S_IFDIR:  printf("d"); break;
                case S_IFIFO:  printf("p"); break;
                case S_IFLNK:  printf("l"); break;
                case S_IFSOCK: printf("s"); break;
                default:       printf("-"); break;
            }

            printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

            char *t = ctime(&fileStat.st_ctime);
            if (t[strlen(t) - 1] == '\n')
                t[strlen(t) - 1] = '\0';

            pw = getpwuid(fileStat.st_uid);
            gr = getgrgid(fileStat.st_gid);

            printf(" %8s %8s %8d %8s  %s\n", pw->pw_name, gr->gr_name, fileStat.st_size, t, d->dName);
            bpos += d->dReclen;
        }
    }

    _exit(0);
}
