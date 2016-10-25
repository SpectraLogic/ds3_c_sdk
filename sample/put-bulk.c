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
#include <unistd.h>
#include <sys/stat.h>
#include "ds3.h"
#include "samples.h"

int main(void) {

    // The bucket the files will be stored in
    const char* bucket_name = BUCKETNAME; // defined in samples.h

    // A list of files to bulk put
    const char* books[4] = BOOKS; // defined in samples.h

    // Get a client instance which uses the environment variables to get the endpoint and credentials
    ds3_client* client;
    ds3_request* request;
    ds3_error* error;
    ds3_bulk_object_list_response* obj_list;
    ds3_master_object_list_response* chunks_response;
    ds3_bulk_object_response* current_obj_to_put;
    uint64_t chunk_count, current_chunk_count = 0;
    uint64_t chunk_index, obj_index;
    FILE* obj_file;

    // Create a client from environment variables
    error = ds3_create_client_from_env(&client);
    handle_error(error);

    // Create a bucket where our files will be stored
    request = ds3_init_put_bucket_request(bucket_name); // We need to create the request

    error = ds3_put_bucket_request(client, request); // This will send the request
    ds3_request_free(request);
    handle_error(error);

    // This takes a list of files and creates ds3 object structs.  The files
    // must exist as is so that the call can get their sizes.  If the files have their
    // names translated as objects, this call must be performed manually
    obj_list = ds3_convert_file_list(books, 4);

    // Initialize the bulk put request
    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);

    // Send the bulk put request to the server
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &chunks_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(obj_list);
    handle_error(error);

    chunk_count = chunks_response->num_objects;

    // Bulk jobs are split into multiple chunks which then need to be transferred
    while (current_chunk_count < chunk_count) {

        // Get the chunks that the server can receive.  The server may not be able to receive everything, so not all chunks will necessarily be returned 
        request = ds3_init_get_job_chunks_ready_for_client_processing_spectra_s3_request(chunks_response->job_id->value);
        error = ds3_get_job_chunks_ready_for_client_processing_spectra_s3_request(client, request, &chunks_response);
        ds3_request_free(request);
        handle_error(error);

        // Check to see if any chunks can be processed
        if (chunks_response->num_objects > 0) {
            // Loop through all the chunks that are avaiable for processing, and send the files that are contained in them
            for (chunk_index = 0; chunk_index < chunks_response->num_objects; chunk_index++) {
                ds3_objects_response* chunk_object_list = chunks_response->objects[chunk_index];
                for (obj_index = 0; obj_index < chunk_object_list->num_objects; obj_index++) {
                    current_obj_to_put = chunk_object_list->objects[obj_index];

                    request = ds3_init_put_object_request(bucket_name, current_obj_to_put->name->value, current_obj_to_put->length);
                    ds3_request_set_job(request, chunks_response->job_id->value);
                    ds3_request_set_offset(request, current_obj_to_put->offset);
                    obj_file = fopen(current_obj_to_put->name->value, "r");
                    error = ds3_put_object_request(client, request, obj_file, ds3_read_from_file);
                    ds3_request_free(request);
                    fclose(obj_file);
                    handle_error(error);
                }
                current_chunk_count++;
            }
        }
        else {
            // When no chunks are returned we need to sleep to allow for cache space to be freed
            sleep(300);
        }
    }

    // Cleanup the client and sdk
    ds3_master_object_list_response_free(chunks_response);
    ds3_creds_free(client->creds);
    ds3_client_free(client);
    ds3_cleanup();
    return 0;
}

