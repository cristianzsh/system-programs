#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>

void help(int exitStatus) {
    printf("Usage: ./uptime [OPTIONS]\n");
    printf("Display how long the machine has been on.\n");
    printf("Options:\n");
    printf(" -a \tshow information about memory\n");
    _exit(exitStatus);
}

int main(int argc, char *args[]) {
    if (argc == 2 && (strcmp(args[1], "--help") == 0))
        help(0);

    if (argc > 2 || (argc == 2 && (strcmp(args[1], "-a") != 0)))
        help(1);

    const long m = 60;
    const long h = m * 60;
    const long d = h * 24;
    const double mb = 1024 * 1024;
    struct sysinfo info;
    sysinfo(&info);

    printf("up %d days, %d hour(s) and %d minute(s)\n",
            info.uptime / d,
            (info.uptime % d) / h,
            (info.uptime % h) / m);

    if (argc == 2 && strcmp(args[1], "-a") == 0)
        printf("Total RAM: %5.1f MB\nAvailable RAM: %5.1f MB\n",
            info.totalram / mb,
            info.freeram / mb);

    _exit(0);
}
