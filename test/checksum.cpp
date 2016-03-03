#include "checksum.h"
#include <sys/types.h>
#include <sys/stat.h>
#if 0
#include <sys/mman.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//global variables
gchar* result_1;
gchar* result_2;

// Get the size of the file by its file descriptor
unsigned long get_size_by_fd(int fd) {
    struct stat statbuf;
    if(fstat(fd, &statbuf) < 0) exit(-1);
    return statbuf.st_size;
}

// Function which compares checksums of the files passed
bool compare_hash_extended(char* filename_1, char* filename_2, unsigned long num_bytes_to_check, unsigned long offset_1, unsigned long offset_2) {
    GMappedFile* file_descript_1;
    GMappedFile* file_descript_2;
    GError *error=NULL;
    char* file_buffer_1;
    char* file_buffer_2;

    if(!filename_1 || !filename_2) {
        printf("Must specify two files to be compared\n");
        exit(-1);
    }
    printf("Original file:\t%s\n", filename_1);
    printf("File to be checked:\t%s\n", filename_2);

    file_descript_1 = g_mapped_file_new (filename_1, FALSE, &error);
    if(error!=NULL) exit(-1);
    
    file_descript_2 = g_mapped_file_new (filename_2, FALSE, &error);
    if(error!=NULL) exit(-1);

    file_buffer_1 = g_mapped_file_get_contents(file_descript_1);
    file_buffer_2 = g_mapped_file_get_contents(file_descript_2);
    if(num_bytes_to_check==0) {
        num_bytes_to_check=MIN(g_mapped_file_get_length (file_descript_1), g_mapped_file_get_length (file_descript_1));
    }
    
    result_1 = g_compute_checksum_for_string(G_CHECKSUM_MD5,file_buffer_1+offset_1, num_bytes_to_check);
    printf("%s(checksum):",filename_1);
    printf("%s\n",result_1);
    
    result_2 = g_compute_checksum_for_string(G_CHECKSUM_MD5,file_buffer_2+offset_2, num_bytes_to_check);
    printf("%s(checksum):",filename_2);
    printf("%s\n",result_2);
    
    bool passed = strcmp(reinterpret_cast<char*>(result_1),reinterpret_cast<char*>(result_2))==0;
    if (passed) {
        printf("Data Integrity Test Passed...MD5 Checksum is Same for Both Files\n");
    } else {
        printf("Data Integrity Test Failed...MD5 Checksum is Not Same for Both Files\n");
    }

    g_free(result_1);
    g_free(result_2);

    g_mapped_file_unref(file_descript_1);
    g_mapped_file_unref(file_descript_2);

    return passed;
}

// Function which compares checksums of the files passed
bool compare_hash(char* filename_1, char* filename_2) {
    return compare_hash_extended(filename_1, filename_2, 0, 0, 0);
}
