#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <sys/stat.h>
#include <boost/test/unit_test.hpp>
#include "ds3.h"
#include "ds3_net.h"
#include "test.h"

#define BUFF_SIZE 16
ds3_bulk_object_list_response* create_object_list_10k_very_small_files(const char* file_name) {
    char put_filename[BUFF_SIZE];
    size_t num_files = 10000;

    struct stat file_info;
    memset(&file_info, 0, sizeof(struct stat));
    stat(file_name, &file_info);
    printf("%s: %ld\n", file_name, file_info.st_size);

    ds3_bulk_object_list_response* obj_list = ds3_init_bulk_object_list();

    GPtrArray* ds3_bulk_object_response_array = g_ptr_array_new();
    for (size_t index = 0; index < num_files; index++) {
        g_snprintf(put_filename, BUFF_SIZE, "file_%05lu", index);

        ds3_bulk_object_response* obj = g_new0(ds3_bulk_object_response, 1);
        obj->name = ds3_str_init(put_filename);
        obj->length = file_info.st_size;
        g_ptr_array_add(ds3_bulk_object_response_array, obj);
    }

    obj_list->objects = (ds3_bulk_object_response**)ds3_bulk_object_response_array->pdata;
    obj_list->num_objects = ds3_bulk_object_response_array->len;
    g_ptr_array_free(ds3_bulk_object_response_array, FALSE);

    return obj_list;
}

BOOST_AUTO_TEST_CASE( bulk_put_10k_very_small_files ) {
    printf("-----Testing Bulk PUT of 10k very small files-------\n");
    ds3_request* request = NULL;
    const char* bucket_name = "test_bulk_put_10k_very_small_files_bucket";
    const char* object_name = "resources/very_small_file.txt";
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_bulk_object_list_response* object_list = create_object_list_10k_very_small_files(object_name);
    ds3_client* client = get_client();

    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, object_list);

    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);
    handle_error(error);

    ds3_master_object_list_response* chunk_response = ensure_available_chunks(client, bulk_response->job_id);

    for (int chunk_index = 0; chunk_index < chunk_response->num_objects; chunk_index++) {
        ds3_objects_response* chunk_object_list = chunk_response->objects[chunk_index];
        for (int object_index = 0; object_index < chunk_object_list->num_objects; object_index++) {
            ds3_bulk_object_response* current_obj = chunk_object_list->objects[object_index];
            FILE* file = fopen(object_name, "r");
            if (file == NULL) {
                printf("FILE NULL!\n");
            }

            request = ds3_init_put_object_request(bucket_name, current_obj->name->value, current_obj->length);
            ds3_request_set_job(request, bulk_response->job_id->value);
            ds3_request_set_offset(request, current_obj->offset);
            if (current_obj->offset > 0) {
                fseek(file, current_obj->offset, SEEK_SET);
            }
            if (object_index % 1000 == 0) {
                printf("PUT'ing %s: %ld\n", current_obj->name->value, current_obj->length);
            }
            error = ds3_put_object_request(client, request, file, ds3_read_from_file);
            ds3_request_free(request);

            fclose(file);
            handle_error(error);
        }
    }

    ds3_master_object_list_response_free(chunk_response);
    ds3_master_object_list_response_free(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}
