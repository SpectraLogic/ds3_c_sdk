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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ds3.h"
#include "samples.h"

int main(void) {
      
    // The bucket the files will be stored in      
    const char* bucket_name = BUCKETNAME;
    
    // A list of files to bulk put
    const char* books[4] = BOOKS;

    // Get a client instance which uses the environment variables to get the endpoint and credentials
    ds3_client* client;
    ds3_request* request;
    ds3_error* error;
    ds3_bulk_object_list* obj_list;
    ds3_get_available_chunks_response* chunks_response;
    ds3_bulk_response* response;
    ds3_bulk_object_list* bulk_obj_list;
    ds3_bulk_object bulk_obj;
    uint64_t chunk_count, current_chunk_count = 0;
    uint64_t chunk_index, obj_index;
    FILE* obj_file;
    
    // Create a client from environment variables
    error = ds3_create_client_from_env(&client);
    handle_error(error);

    // Create a bucket where our files will be stored
    request = ds3_init_put_bucket(bucket_name); // We need to create the request
    
    error = ds3_put_bucket(client, request); // This will send the request
    ds3_free_request(request);
    handle_error(error);

    // Create the bulk put request
    obj_list = ds3_convert_file_list(books, 4); // This takes a list of files and creates ds3 object structs.  The files
                                                // must exist as is so that the call can get their sizes.  If the files have their
                                                // names translated as objects, this call must be performed manually
    request = ds3_init_put_bulk(bucket_name, obj_list); // Creating the request that will be the bulk put

    // Initialize the bulk put
    error = ds3_bulk(client, request, &response); // Sends the bulk put request to the server
    ds3_free_request(request);
    handle_error(error);

    chunk_count = response->list_size;

    // Bulk jobs are split into multiple chunks which then need to be transferred
    while (current_chunk_count < chunk_count) {
        
        // Get the chunks that the server can receive.  The server may not be able to receive everything, so not all chunks will necessarily be returned 
        request = ds3_init_get_available_chunks(response->job_id->value);
        error = ds3_get_available_chunks(client, request, &chunks_response);
        ds3_free_request(request);
        handle_error(error);

        // Check to see if any chunks can be processed
        if (chunks_response->object_list->list_size > 0) {
            // Loop through all the chunks that are avaiable for processing, and send the files that are contained in them
            for (chunk_index = 0; chunk_index < chunks_response->object_list->list_size; chunk_index++) {
                bulk_obj_list = chunks_response->object_list->list[chunk_index];
                
                // Check to make sure that we have not already processed this chunk.  It's possible that the 
                // get_available_chunk api call will return the same chunk on subsequent calls. 
                if (bulk_obj_list->chunk_number < current_chunk_count) {
                    continue;  // do not process a chunk we've already processed
                }
                for (obj_index = 0; obj_index < bulk_obj_list->size; obj_index++) {
                    bulk_obj = bulk_obj_list->list[obj_index];
                    request = ds3_init_put_object_for_job(bucket_name, bulk_obj.name->value, bulk_obj.offset, bulk_obj.length, response->job_id->value);
                    obj_file = fopen(bulk_obj.name->value, "r");
                    error = ds3_put_object(client, request, obj_file, ds3_read_from_file);
                    ds3_free_request(request);
                    fclose(obj_file);
                    handle_error(error);
                }
                current_chunk_count++;
            }
        }
        else {
            // When no chunks are returned we need to sleep to allow for cache space to be freed
            sleep(chunks_response->retry_after);
        }
    }

    // Cleanup the client and sdk
    ds3_free_creds(client->creds);
    ds3_free_client(client);
    ds3_cleanup();
    return 0;
}
