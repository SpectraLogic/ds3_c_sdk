#include "checksum.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
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
bool compare_hash(char* filename_1, char* filename_2, unsigned long num_bytes_to_check) {
    int file_descript_1;
    int file_descript_2;
    unsigned long file_size_1,file_size_2;
    char* file_buffer_1;
    char* file_buffer_2;

    if(!filename_1 || !filename_2) {
        printf("Must specify two files to be compared\n");
        exit(-1);
    }
    printf("Orignal file:\t%s\n", filename_1);
    printf("File to be checked:\t%s\n", filename_2);

    file_descript_1 = open(filename_1, O_RDONLY);
    if(file_descript_1 < 0) exit(-1);

    file_descript_2 = open(filename_2, O_RDONLY);
    if(file_descript_2 < 0) exit(-1);

    file_size_1 = get_size_by_fd(file_descript_1);
    file_size_2 = get_size_by_fd(file_descript_2);
    if(num_bytes_to_check != 0) {
        if(file_size_1 > num_bytes_to_check) {
            file_size_1 = num_bytes_to_check;
        }
        if(file_size_2 > num_bytes_to_check) {
            file_size_2 = num_bytes_to_check;
        }
    }

    file_buffer_1 = static_cast<char*>(mmap(0, file_size_1, PROT_READ, MAP_SHARED, file_descript_1, 0));
    result_1 = g_compute_checksum_for_string(G_CHECKSUM_MD5,file_buffer_1,file_size_1);
    printf("%s(checksum):",filename_1);
    printf("%s\n",result_1);

    file_buffer_2 = static_cast<char*>(mmap(0, file_size_2, PROT_READ, MAP_SHARED, file_descript_2, 0));
    result_2 = g_compute_checksum_for_string(G_CHECKSUM_MD5,file_buffer_2,file_size_2);
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


    return passed;
}

