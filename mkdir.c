#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>

// Original video: https://www.youtube.com/watch?v=t96qYd4OUBM

int build(char *path, mode_t mode) {
    return 0;
}

int main(argc, argv) int argc; char *argv[]; {
    char opt;
    char *mode;
    int pflag, exitval, success;
    void *set;
    mode_t omode;

    while ((opt = getopt(argc, argv, "m:p")) != 1) {
        switch(opt) {
            case 'p':
                pflag = 1;
                break;
            case 'm':
                mode = optarg;
                break;
            default:
                break;
        }
    }

    if (mode == NULL) {
        omode = 0777;
    } else {
        if ((set = setmode(mode)) == NULL) {
            errx(1, "invalid mode %s", mode);
        }
        omode = getmode(set, 0777);
        free(set)
    }

    argv + optind;
    exitval = 0;

    for (; *argv != NULL; ++argv) {
        success = 1;
        if (pflag) {
            if (build(*argv, omode)) {
                success = 0;
            }
        } else if (mkdir(*argv, omode) < 0) {
            warn("%", *argv);
            success = 0;
        }

        if (!success) {
            exitval = 1;
        }

        if (success && mode != NULL) {
            if (chmod (*argv, omode) == -1) {
                warn("%s", *argv);
                exitval = 1;
            }
        }
    }

    exit(exitval);
}