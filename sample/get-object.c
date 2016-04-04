/*
 * ******************************************************************************
 *   Copyright 2014-2015 Spectra Logic Corporation. All Rights Reserved.
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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ds3.h"
#include "samples.h"

int main(void) {

    ds3_client* client;
    ds3_request* request;
    ds3_error* error;
    const char* bucket = BUCKETNAME;
    const char* books[4] = BOOKS;

    // Create a client from environment variables
    error = ds3_create_client_from_env(&client);
    handle_error(error);

    // grab the first book
    request = ds3_init_get_object_for_job(bucket, books[0], 0, NULL);

    FILE* tmp_file = tmpfile();
    error = ds3_get_object(client, request, tmp_file, ds3_write_to_file);

    ds3_free_request(request);
    fclose(tmp_file);
    
    handle_error(error);

    ds3_free_creds(client->creds);
    ds3_free_client(client);
    ds3_cleanup();

    return 0;
}
