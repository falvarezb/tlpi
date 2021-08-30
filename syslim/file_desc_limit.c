#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <stdint.h>

/**
 * code to create 'myfiles' folder
 *
 * Program to simulate error EMFILE when too many files are held open by a process
 * It takes 2 parameters:
 * - number of files to open (it cannot be greater than MAX_NUM_FILES)
 * - optional parameter to set the soft limit of the maximum number of files that can be
 * held open by the process. If not provided, the default value set in the environment where the
 * application runs is used
 *
 * Files are created in a folder called 'myfiles' in the cwd. The folder is created if
 * it does not exist
 *
 * Note: given that all processes open 3 files by default (stdin, stdout and stderr), that must be
 * taken into consideration when specifying the number of files to open
 * 
 * HOW TO COMPILE
 * ---> gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0, ldd (Ubuntu GLIBC 2.27-3ubuntu1) 2.27
 * /usr/bin/gcc -D_XOPEN_SOURCE=600 ./file_desc_limit.c -o ./out/file_desc_limit
 * ---> Darwin Kernel Version 20.6.0, Apple clang version 12.0.5 (clang-1205.0.22.11)
 * /usr/bin/gcc ./file_desc_limit.c -o ./out/file_desc_limit
 *
 */

// maximum allowed number of files to open
// ATTENTION: if changed, it may be necessary to review the value of BUF_SIZE_OFFSET too
#define MAX_NUM_FILES 20000

// constant used to set size of buffers used in the program
// it represents the size in chars of the greatest file descriptor used in the program
// it goes hand in hand with MAX_NUM_FILES
#define BUF_SIZE_OFFSET "5"

#define MY_FILES "myfiles"

#define errExit(msg) do { \
    perror(msg); exit(EXIT_FAILURE); \
} while (0)

/**
 * Update soft limit of resource RLIMIT_NOFILE to 'value'
 */
void update_nofiles_soft_limit(char const* value, struct rlimit rl) {
    int new_soft_limit_nofile = atoi(value);
    rl.rlim_cur = new_soft_limit_nofile;
    if (setrlimit(RLIMIT_NOFILE, &rl) == -1) {
        perror("setrlimit");
    }

    // rlim_t is a long int in Linux and long long int in Apple
    // to handle both cases, we cast the value to uintmax_t and use the corresponding format specifier
    printf("new soft limit of RLIMIT_NOFILE:%ju\n", (uintmax_t) rl.rlim_cur);
}

/**
 * Return a buffer containing the symlink /proc/self/fd/'fd' of the file whose file descriptor is 'fd' 
 */
char* fd_symlink(int fd, const size_t fd_size) {
    const size_t link_name_buf_size = strlen("/proc/self/fd/") + fd_size;
    char* link_name_buf = (char*)malloc(link_name_buf_size);
    if (link_name_buf == NULL) {
        errExit("insufficient memory to create link_name\n");        
    }
    sprintf(link_name_buf, "/proc/self/fd/%d", fd);
    return link_name_buf;
}

/**
 * Return target of the symlink contained in the buffer 'link_name_buf'
 */
char* read_link(char* link_name_buf) {
    struct stat sb;

    if (lstat(link_name_buf, &sb) == -1) {
        errExit("lstat");        
    }

    //sb.st_size given by 'lstat' is the length of the symlink's target's pathname
    char* link_target = (char*)malloc(sb.st_size + 1);
    if (link_target == NULL) {
        errExit("insufficient memory to create link_target\n");        
    }

    // sb.st_size is the expected size of the target's pathname
    // readlink returns the actual size
    ssize_t actual_size = readlink(link_name_buf, link_target, sb.st_size);
    if (actual_size < 0) {
        errExit("readlink");        
    }

    if (actual_size > sb.st_size) {
        perror("symlink increased in size between lstat() and readlink(), value of link_target has been truncated\n");
        printf("original value %jd, new value %ld\n", (uintmax_t) sb.st_size, actual_size);
        exit(EXIT_FAILURE);
    }

    link_target[sb.st_size] = '\0';
    return link_target;
}

int main(int argc, char const* argv[])
{
    // num-files must be <= (new-soft-limit - 3) to account for stdin, stdout and stderror
    // otherwise program with fail with error EMFILE
    if ((argc != 2 && argc != 3) || strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "Usage: %s num-files [new-soft-limit] \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    size_t num_files = atoi(argv[1]);
    if(num_files > MAX_NUM_FILES){
        fprintf(stderr, "%zu cannot be greater than %d\n", num_files, MAX_NUM_FILES);
        exit(EXIT_FAILURE);
    }    

    struct rlimit rl;
    if (getrlimit(RLIMIT_NOFILE, &rl) == -1) {
        perror("getrlimit");
    }
    printf("soft limit of RLIMIT_NOFILE:%ju\n", (uintmax_t) rl.rlim_cur);

    // updating soft limit of RLIMIT_NOFILE
    if (argc == 3) {
        update_nofiles_soft_limit(argv[2], rl);
    }

    const size_t buf_size_offset = atoi(BUF_SIZE_OFFSET);
    const char* filename = MY_FILES "/f_%0" BUF_SIZE_OFFSET "zu";
    const size_t filename_buf_size = strlen(MY_FILES "/f_") + buf_size_offset;

    // creating folder
    if(mkdir(MY_FILES, S_IRUSR | S_IWUSR | S_IXUSR) == -1 && errno != EEXIST){
        errExit("mkdir");        
    }

    // creating/opening files
    size_t i;
    for (i = 0; i < num_files; i++)
    {
        char filename_buf[filename_buf_size];
        sprintf(filename_buf, filename, i);
        int inputFd = open(filename_buf, O_CREAT | O_RDONLY, S_IRUSR);
        if (inputFd == -1)
        {
            printf("error while opening file %s\n", filename_buf);
            errExit("open");
        }
    }

    // print out pathnames of newly created files (using fd as parameter)
    // fd<i+3 to account for stdin, stdout and stderror
    for (size_t fd = 0; fd < i + 3; fd++)
    {
#ifdef __linux__
        // pathnames are obtained from the symlinks /proc/self/fd/n
        char* link_name_buf = fd_symlink(fd, buf_size_offset);
        printf("fd=%zu, path=%s\n", fd, read_link(link_name_buf));
#endif

#ifdef __APPLE__
        // F_GETPATH doesn't exist in Linux
        char file_path[PATH_MAX];
        if (fcntl(fd, F_GETPATH, file_path) == -1)
        {
            errExit("fcntl error");            
        }

        printf("fd=%zu, path=%s\n", fd, file_path);
#endif
    }
    exit(EXIT_SUCCESS);
}