
/* t_writev.c

   Demonstrate the use of the writev() system call to perform "gather output".
*/

#include <sys/uio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define IOVEC_SIZE 12

void errorExit(char *format, const char *text) {
    printf(format, errno, text);
    perror("");
    exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
    size_t fd, i, totRequired;
    struct iovec iov[IOVEC_SIZE];
    uint8_t ux8 = 10;
    uint16_t ux16 = 10;
    uint32_t ux32 = 10;
    uint64_t ux64 = 10;
    int8_t x8 = -128;
    int16_t x16 = -128;                     
    int32_t x32 = -128;                     
    int64_t x64 = -128;
    float f = 10;
    double d = 10;                  
    char *str = "hello";   
    struct __attribute__((__packed__)) {
        char first;
        char second;
        char third;
        int fourth;
    } mystruct = {'a', 'b', 'c', 16};   

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        printf("%s file\n", argv[0]);

    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1)
        errorExit("error %d while opening file %s", argv[1]);

    totRequired = 0;

    i = 0;
    iov[i].iov_base = &ux8;
    iov[i].iov_len = sizeof(ux8);
    totRequired += iov[i].iov_len;

    i = 1;
    iov[i].iov_base = &ux16;
    iov[i].iov_len = sizeof(ux16);
    totRequired += iov[i].iov_len;

    i = 2;
    iov[i].iov_base = &ux32;
    iov[i].iov_len = sizeof(ux32);
    totRequired += iov[i].iov_len;

    i = 3;
    iov[i].iov_base = &ux64;
    iov[i].iov_len = sizeof(ux64);
    totRequired += iov[i].iov_len;

    i = 4;
    iov[i].iov_base = &x8;
    iov[i].iov_len = sizeof(x8);
    totRequired += iov[i].iov_len;

    i = 5;
    iov[i].iov_base = &x16;
    iov[i].iov_len = sizeof(x16);
    totRequired += iov[i].iov_len;

    i = 6;
    iov[i].iov_base = &x32;
    iov[i].iov_len = sizeof(x32);
    totRequired += iov[i].iov_len;

    i = 7;
    iov[i].iov_base = &x64;
    iov[i].iov_len = sizeof(x64);
    totRequired += iov[i].iov_len;

    i = 8;
    iov[i].iov_base = &f;
    iov[i].iov_len = sizeof(f);
    totRequired += iov[i].iov_len;

    i = 9;
    iov[i].iov_base = &d;
    iov[i].iov_len = sizeof(d);
    totRequired += iov[i].iov_len;

    i = 10;
    iov[i].iov_base = str;
    iov[i].iov_len = strlen(str);
    totRequired += iov[i].iov_len;

    i = 11;
    iov[i].iov_base = &mystruct;
    iov[i].iov_len = sizeof(mystruct);
    totRequired += iov[i].iov_len;


    if(writev(fd, iov, IOVEC_SIZE) != totRequired) {
        perror("write() returned error or partial write occurred");
        exit(EXIT_FAILURE);
    }

    printf("%zu totRequired", totRequired);

    if (close(fd) == -1) perror("close input");

    exit(EXIT_SUCCESS);
}
/*
Things to learn:
0a 
0a 00  <-- little endian representation: least significant bytes come first
0a 00 00 00 
0a 00 00 00 00 00 00 00 
80 
80 ff 
80 ff ff ff 
80 ff ff ff ff ff ff ff 
00 00 20 41 
00 00 00 00 00 00 24 40 
68 65 6c 6c 6f 
61 62 63 00 10 00 00 00 <-- padding with zero byte (the one after 63) for memory alignment, word length is 4 bytes
*/