//Header Guard
#ifndef __CHECKSUM__
#define __CHECKSUM__

//Dependencies
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>



//Functions
unsigned long get_size_by_fd(int fd);
void compare_hash(char* filename_1, char* filename_2);

#endif
