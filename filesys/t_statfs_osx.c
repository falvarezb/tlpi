/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Supplementary program for Chapter 14 */

/* t_statfs.c

   Demonstrate the use of statfs() to retrieve information about
   a mounted file system.

   This program is Linux-specific.

   See also t_statvfs.c.
*/
#include <sys/param.h>
#include <sys/mount.h>
#include "tlpi_hdr.h"

int
main(int argc, char* argv[])
{
        struct statfs sfs;

        if (argc != 2 || strcmp(argv[1], "--help") == 0)
                usageErr("%s path\n", argv[0]);

        if (statfs(argv[1], &sfs) == -1)
                errExit("statfs");

        printf("File system type: %#lx\n", (unsigned long)sfs.f_type);
        printf("File system type name:                  %s\n", sfs.f_fstypename);
        printf("File system mounted on:                  %s\n", sfs.f_mntonname);
        printf("Mounted file system:                  %s\n", sfs.f_mntfromname);
        printf("Fundamental I/O block size:        %lu\n", (unsigned long)sfs.f_bsize);
        printf("Optimal I/O block size:        %lu\n", (unsigned long)sfs.f_iosize);
        printf("Total data blocks:             %lu\n", (unsigned long)sfs.f_blocks);
        printf("Free data blocks:              %lu\n", (unsigned long)sfs.f_bfree);
        printf("Free blocks for nonsuperuser:  %lu\n", (unsigned long)sfs.f_bavail);
        printf("Total i-nodes:                 %lu\n", (unsigned long)sfs.f_files);
        printf("Free i-nodes:                  %lu\n", (unsigned long)sfs.f_ffree);
        printf("Free i-nodes:                  %lu\n", (unsigned long)sfs.f_ffree);
        printf("mount flags:                  %lu\n", (unsigned long)sfs.f_flags);
        printf("owner:                  %lu\n", (unsigned long)sfs.f_owner);
        

        exit(EXIT_SUCCESS);
}
