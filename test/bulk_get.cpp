#include <stdio.h>
#include <unistd.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

#define FILE_TEMPLATE "bulk-XXXXXX"

BOOST_AUTO_TEST_CASE( bulk_get ) {
    uint64_t i, n;
    uint64_t file_index = 0;
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_get_bucket_response* response = NULL;
    ds3_bulk_response* bulk_response = NULL;
    ds3_bulk_object_list* object_list = NULL;
    ds3_get_available_chunks_response* chunk_response = NULL;
    bool retry_get;

    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";

    char** tmp_files;

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &response);

    ds3_free_request(request);

    BOOST_REQUIRE(error == NULL);

    tmp_files = (char**) calloc(response->num_objects, sizeof(char*));

    object_list = ds3_convert_object_list(response->objects, response->num_objects);

    request = ds3_init_get_bulk(bucket_name, object_list, NONE);
    error = ds3_bulk(client, request, &bulk_response);

    ds3_free_request(request);
    ds3_free_bulk_object_list(object_list);

    BOOST_REQUIRE(error == NULL);

    do {
        retry_get = false;
        request = ds3_init_get_available_chunks(bulk_response->job_id->value);

        error = ds3_get_available_chunks(client, request, &chunk_response);

        ds3_free_request(request);

        BOOST_REQUIRE(error == NULL);

        BOOST_REQUIRE(chunk_response != NULL);

        if (chunk_response->retry_after != 0) {
            // if this happens we need to try the request
            retry_get = true;
            sleep(chunk_response->retry_after);
        }

    } while(retry_get);

    BOOST_REQUIRE(error == NULL);

    for (i = 0; i < chunk_response->object_list->list_size; i++) {
        ds3_bulk_object_list* chunk_object_list = chunk_response->object_list->list[i];
        for(n = 0; n < chunk_object_list->size; n++, file_index++) {
            FILE* w_file;
            ds3_bulk_object current_obj = chunk_object_list->list[n];
            request = ds3_init_get_object(bucket_name, current_obj.name->value);
            tmp_files[file_index] = (char*) calloc(13, sizeof(char));
            memcpy(tmp_files[file_index], FILE_TEMPLATE, 12);
            w_file = fopen(tmp_files[file_index], "w+");
            error = ds3_get_object(client, request, w_file, ds3_write_to_file);
            fclose(w_file);
        }
    }

    for (i = 0; i <= file_index;i++) {
        unlink(tmp_files[i]);
    }

    free(tmp_files);

    ds3_free_available_chunks_response(chunk_response);
    ds3_free_bulk_response(bulk_response);

    clear_bucket(client, bucket_name);
}
