//Header Guard
#ifndef __CHECKSUM__
#define __CHECKSUM__

//Dependencies
#include <glib.h>



//Functions
unsigned long get_size_by_fd(int fd);
void compare_hash(char* filename_1, char* filename_2, unsigned long num_bytes_to_check = 0);

#endif
