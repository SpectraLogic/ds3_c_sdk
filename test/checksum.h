//Header Guard
#ifndef __CHECKSUM__
#define __CHECKSUM__

//Dependencies
#include <glib.h>



//Functions
unsigned long get_size_by_fd(int fd);
bool compare_hash(char* filename_1, char* filename_2);

#endif
