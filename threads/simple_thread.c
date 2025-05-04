/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 29-1 */

/* simple_thread.c

   A simple POSIX threads example: create a thread, and then join with it.
*/
#include <pthread.h>
#include "tlpi_hdr.h"

static void *
threadFunc(void *arg)
{
    sleep(5);
    char *s = (char *) arg;

    printf("%s", s);

    return (void *) strlen(s);
}

static void * threadDoNothing(void *arg) {return NULL;}
static void * detachedThread(void *arg) {
    pthread_detach(pthread_self());
    return NULL;
}
static void * threadExit(void *arg) {
    printf("message from thread before exit\n");    
    exit(EXIT_SUCCESS);
}
static void * threadSleep(void *arg) {
    pthread_detach(pthread_self());
    sleep(30);
    return NULL;
}

/**
 * main thread waits and joins child thread
 */
int case1() {
    pthread_t t1;
    void *res;    

    int s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
    if (s != 0)
        errExitEN(s, "pthread_create");

    printf("Message from main()\n");
    s = pthread_join(t1, &res);
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("Thread returned %ld\n", (long) res);

    exit(EXIT_SUCCESS);
}

/**
 * main thread ends its execution without waiting for child to finish and the entire process finishes
 */
int case2() {
    pthread_t t1;      

    int s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
    if (s != 0)
        errExitEN(s, "pthread_create");

    printf("Message from main()\n");
    return 0;
}

/**
 * main thread exits but child continues running
 */
int case3() {
    pthread_t t1;      

    int s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
    if (s != 0)
        errExitEN(s, "pthread_create");

    printf("Message from main()\n");
    pthread_exit(NULL);
}

/**
 * creation of zombie threads (child threads that are not joined by some other thread)
 */
void case4() {
    pthread_t t1;      
    int num_threads = 100000;
    for (int i = 0; i < num_threads; i++){
        int s = pthread_create(&t1, NULL, threadDoNothing, NULL);
        if (s != 0)
            errExitEN(s, "pthread_create");

        if(i%(num_threads/10) == 0) {
          printf("New thread created - %d\n", i);
        }
        //sleep(1);
    }
    printf("Message from main()\n");
}

/**
 * creation of detached threads (child threads that do not need to be joined)
 */
void case5() {
    pthread_t t1;      
    int num_threads = 100000;
    for (int i = 0; i < num_threads; i++){
        int s = pthread_create(&t1, NULL, detachedThread, NULL);
        if (s != 0)
            errExitEN(s, "pthread_create");

        if(i%(num_threads/10) == 0) {
          printf("New thread created - %d\n", i);
        }
        //sleep(1);
    }
    printf("Message from main()\n");
}

/**
 * an exit by any thread finishes all other threads and the entire process
 */
void case6() {
    pthread_t t1;          
    
    int s = pthread_create(&t1, NULL, threadExit, NULL);
    if (s != 0)
        errExitEN(s, "pthread_create");

    //sleep to give time for the child thread to exit
    sleep(5);    
    printf("This is never written\n");
}

/**
 * thread joining with itself
 */
void case7() {  
    //it does not block (it does in Java though)       
    pthread_join(pthread_self(), NULL);
    printf("Joining with itself is ok\n");
}

/**
 * exploring limit to the number of threads that can be created
 */
void case8(size_t stack_size) {        
    pthread_t t1;      
    pthread_attr_t attr;

    int s = pthread_attr_init(&attr);       /* Assigns default values */
    if (s != 0)
        errExitEN(s, "pthread_attr_init");

    size_t actual_stack_size;
    
    if (stack_size > 0){        
        pthread_attr_setstacksize(&attr, stack_size);
        if (s != 0)
            errExitEN(s, "pthread_attr_setstacksize");
    }
    pthread_attr_getstacksize(&attr, &actual_stack_size);
    printf("actual_stack_size=%ld\n", actual_stack_size);

    int num_threads = 10000;
    for (int i = 0; i < num_threads; i++){        
        s = pthread_create(&t1, &attr, threadSleep, NULL);
        if (s != 0)
            errExitEN(s, "pthread_create");        

        if(i%(num_threads/10) == 0) {
          printf("New thread created - %d\n", i);
        }
        //sleep(1);
    }
    printf("Message from main()\n");
}

int
main(int argc, char *argv[])
{
    case8(8192l*1024l*1024l);
    
}

/*
rate of thread creation
100000
java SimpleThread.java  6.03s user 3.92s system 231% cpu 4.300 total
./threads/out/simple_thread  0.62s user 1.70s system 186% cpu 1.241 total
4.3/1.241=3.465

1000000
java SimpleThread.java  54.45s user 38.41s system 246% cpu 37.624 total
./threads/out/simple_thread  5.88s user 16.25s system 208% cpu 10.614 total
37.624/10.614=3.545

3000000
java SimpleThread.java  170.03s user 117.55s system 249% cpu 1:55.28 total
./threads/out/simple_thread  17.97s user 49.53s system 214% cpu 31.461 total
115.28/31.461=3.664

maximum number of threads:
Java and C both around 8000 with a stack around 8Mb, that means about 64Gb, hitting VM size limit
*/
