#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(void) {
    time_t now;
    time(&now);
    printf("%s", ctime(&now));

    _exit(0);
}
