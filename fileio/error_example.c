
#include <sys/types.h>
#include <unistd.h> 
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int inputFd, openFlags;
    mode_t filePerms;    

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */

    inputFd = open("", openFlags, filePerms);
    if (inputFd == -1) {
        printf("errno: %d", errno);
        perror("boom");
    }

    close(inputFd);
    
}
