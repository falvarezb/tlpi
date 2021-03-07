
/* t_writev.c

   Demonstrate the use of the writev() system call to perform "gather output".
*/
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define IOVEC_SIZE 1

int
main(int argc, char *argv[])
{
    int fd;
    struct iovec iov[IOVEC_SIZE];
    int x = 1; 
    long y = 10;                     
    char *str = "hello";         
    ssize_t numWritten, totRequired;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1)
        errExit("open");

    totRequired = 0;

    size_t idx = 1;

    // iov[idx].iov_base = &x;
    // iov[idx].iov_len = sizeof(x);
    // totRequired += iov[idx].iov_len;

    idx = 0;
    iov[idx].iov_base = str;
    iov[idx].iov_len = strlen(str);
    totRequired += iov[idx].iov_len;

    // idx = 2;
    // iov[idx].iov_base = &y;
    // iov[idx].iov_len = sizeof(y);
    // totRequired += iov[idx].iov_len;

    numWritten = writev(fd, iov, IOVEC_SIZE);
    if (numWritten == -1)
        errExit("writev");

    if (numWritten < totRequired)
        printf("Partial write\n");

    close(fd);

    /*FIXME: should use %zd here, and remove (long) cast */
    printf("total bytes requested: %ld; bytes written: %ld\n",
            (long) totRequired, (long) numWritten);
    exit(EXIT_SUCCESS);
}
