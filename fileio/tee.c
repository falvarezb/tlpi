#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int main(int argc, char const *argv[])
{
    int inputFd, outputFd1, outputFd2, outputFd3, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    inputFd = STDIN_FILENO;
    outputFd1 = STDOUT_FILENO;

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    outputFd2 = open("temp2.txt", openFlags, filePerms);
    outputFd3 = open("temp3.txt", openFlags, filePerms);

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        if (write(outputFd1, buf, numRead) != numRead)
            fatal("write() returned error or partial write occurred1");
        if (write(outputFd2, buf, numRead) != numRead)
            fatal("write() returned error or partial write occurred2");
        if (write(outputFd3, buf, numRead) != numRead)
            fatal("write() returned error or partial write occurred3");
    }
    if (numRead == -1)
        errExit("read");

    if (close(outputFd2) == -1)
        errExit("close output outputFd2");
    if (close(outputFd3) == -1)
        errExit("close output outputFd3");

    exit(EXIT_SUCCESS);
}
