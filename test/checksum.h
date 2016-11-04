/*
 * ******************************************************************************
 *   Copyright 2014-2016 Spectra Logic Corporation. All Rights Reserved.
 *   Licensed under the Apache License, Version 2.0 (the "License"). You may not use
 *   this file except in compliance with the License. A copy of the License is located at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   or in the "license" file accompanying this file.
 *   This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 *   CONDITIONS OF ANY KIND, either express or implied. See the License for the
 *   specific language governing permissions and limitations under the License.
 * ****************************************************************************
 */

//Header Guard
#ifndef __CHECKSUM__
#define __CHECKSUM__

//Functions
unsigned long get_size_by_fd(int fd);
bool compare_hash_extended(char* filename_1, char* filename_2, unsigned long num_bytes_to_check, unsigned long offset_1, unsigned long offset_2);
bool compare_hash(char* filename_1, char* filename_2);

#endif
