#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void help(int exitStatus) {
    printf("Usage: sudo ./chown [OWNER][:[GROUP]] FILE...\n");
    printf("Change the owner and/or group of each FILE to OWNER and/or GROUP.\n");
    _exit(exitStatus);
}

int getUID(char *usr) {
    struct passwd *pw;

    pw = getpwnam(usr);
    if (pw == NULL)
        return -1;

    return pw->pw_uid;
}

int getGID(char *grp) {
    struct group *gr;

    gr = getgrnam(grp);
    if (gr == NULL)
        return -1;

    return gr->gr_gid;
}

int main(int argc, char *argv[]) {
    if (argc < 3)
        help(1);

    int uid = -1, gid = -1, r;
    char *usr, *grp;

   if (argv[1][0] == ':') {
        gid = getGID(&argv[1][1]);

        if (gid == -1) {
            fprintf(stderr, "Error when trying to get GID!\n");
            _exit(1);
        }
    } else {
        usr = strtok(argv[1], ":");
        grp = strtok(NULL, ":");
        uid = getUID(usr);

        if (uid == -1) {
            fprintf(stderr, "Error when trying to get the UID of %s!\n", usr);
            _exit(1);
        }

        if (grp == NULL) {
            gid = -1;
        } else {
            gid = getGID(grp);

            if (gid == -1) {
                fprintf(stderr, "Error when trying to get the GID of %s!\n", grp);
                _exit(1);
            }
        }
    }

    for (int i = 2; i < argc; i++) {
        r = chown(argv[i], uid, gid);

        if (r < 0) {
            fprintf(stderr, "Error when trying to change permissions for %s!\n", argv[i]);
            _exit(1);
        }
    }

    _exit(0);
}
