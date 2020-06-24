#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

// Original video: https://www.youtube.com/watch?v=p7uJBl4A_BA

int main(argc, argv)
    int argc;
    char *argv[];
{
    char *mode, *path, *ep;
    long mode_val;
    int omode;
    void *set;
    struct stat sbuf;
    mode_t curr_mode;

    if (argc < 2) {
        /* print usage */
        exit(1);
    }
    ++argv;
    mode = *argv;
    ++argv;
    path = *argv;

    if (*mode >= '0' && *mode < '7') {
        errno = 0;
        mode_val = strtol(mode, &ep, 8);
        if (mode_val > INT_MAX || mode_val < 0) {
            errno = ERANGE;
        }
        if (errno) {
            errx(1, "invalid file mode: %s", mode);
        }
        if (*ep) {
            errx(1, "invalid file mode: %s", mode);
        }
        omode = (int)mode_val;
    } else {
        set = setmode(mode);
        if (set == NULL) {
            errx(1, "invalid file mode: %s", mode);
        }

        if(stat(path, &sbuf)) {
            err(1, "%s", path);
        }
        curr_mode = sbuf.st_mode;
        omode = getmode(set, curr_mode);
        free(set);
    }

    if (chmod(path, omode)) {
        warn("%s", path);
    }
}