#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

// Original video: https://www.youtube.com/watch?v=MCuzvy79WWQ

void cat(rfd) int rfd; {
    int wfd;
    static char *buf;
    static size_t bsize;
    struct stat sbuf;

    wfd = fileno(stdout);

    if (!buf) {
        if (fstat(wfd, &sbuf)) {
            err(1, "stdout");
        }
        bsize = sbuf.st_blksize;
        buf = malloc(bsize);

        if (!buf) {
            err(1, 0);
        }
    }

    ssize_t nr, nw;
    int offset = 0;
    nr = read(rfd, buf, bsize);

    while (nr > 0) {
        for (offset = 0; nr > 0; nr -= offset += nw) {
            nw = write(wfd, buf+offset, br);
            if (nw < 0) {
                err(1, "stdout");
            }
        }
         nr = read(rfd, buf, bsize);
    }
}

int main(argc, argv) int argc; char *argv[]; {
    // no flags
    int fd;

    ++argv;
    fd = fileno(stdin);
    
    do {

        if (*argv) {
            if (strcmp(*argv, "-") == 0) {
                fd = fileno(stdin);
            } else {
                fd = open(*argv, O_RDONLY);
            }

            if (fd < 0) {
                err(1, "%s", *argv)
            }
            ++argv;
        }
        cat(fd);
    } while (*argv);

    if (fd != fileno(stdin)) {
        close(fd);
    }
}