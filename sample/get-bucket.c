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
#include <stdio.h>
#include "ds3.h"
#include "samples.h"

int main(void) {

   // Get Bucket
    ds3_client* client;
    ds3_request* request;
    ds3_error* error;
    ds3_list_bucket_result_response* response;
    uint64_t object_index;

    // Create a client from environment variables
    error = ds3_create_client_from_env(&client);
    handle_error(error);

    // Create the get bucket request.
    request = ds3_init_get_bucket_request(BUCKETNAME);

    // This performs the request to a DS3 appliance.
    // If there is an error 'error' will not be NULL
    // If the request completed successfully then 'error' will be NULL
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    if (response == NULL) {
        printf("Response was null\n");
        ds3_client_free(client);
        return 1;
    }

    if(response->num_objects == 0) {
        printf("No objects returned\n");
        ds3_list_bucket_result_response_free(response);
        ds3_client_free(client);
        return 0;
    }

    for (object_index = 0; object_index < response->num_objects; object_index++) {
        ds3_contents_response* object = response->objects[object_index];
        printf("Object: (%s) created on %s\n", object->key->value, object->last_modified->value);
    }

    ds3_list_bucket_result_response_free(response);
    ds3_creds_free(client->creds);
    ds3_client_free(client);
    ds3_cleanup();

    return 0;
}
