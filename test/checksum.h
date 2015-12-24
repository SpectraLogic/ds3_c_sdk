//Header Guard
#ifndef __CHECKSUM__
#define __CHECKSUM__

//Dependencies
#include <glib.h>

//Functions
unsigned long get_size_by_fd(int fd);
bool compare_hash_extended(char* filename_1, char* filename_2, unsigned long num_bytes_to_check, unsigned long offset_1, unsigned long offset_2);
bool compare_hash(char* filename_1, char* filename_2);

#endif
