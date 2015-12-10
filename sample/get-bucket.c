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

   // Get Bucket
    ds3_client* client;
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response *response;
    uint64_t i;

    // Create a client from environment variables
    error = ds3_create_client_from_env(&client);
    handle_error(error);

    // Create the get bucket request.
    request = ds3_init_get_bucket(BUCKETNAME);

    // This performs the request to a DS3 appliance.
    // If there is an error 'error' will not be NULL
    // If the request completed successfully then 'error' will be NULL
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);

    handle_error(error);
	
    if (response == NULL) {
        printf("Response was null\n");
        ds3_free_client(client);
        return 1;
    }

    if(response->num_objects == 0) {
        printf("No objects returned\n");
        ds3_free_bucket_response(response);
        ds3_free_client(client);
        return 0;
    }

    for (i = 0; i < response->num_objects; i++) {
        ds3_object object = response->objects[i];
        printf("Object: (%s) created on %s\n", object.name->value, object.last_modified->value);
    }

    ds3_free_bucket_response(response);

    ds3_free_creds(client->creds);
    ds3_free_client(client);
    ds3_cleanup();

    return 0;
}
