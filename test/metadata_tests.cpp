#include <stdbool.h>
#include <stdio.h>
#include <glib.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( put_metadata ) {
    ds3_error* error;
    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result;
    ds3_metadata_entry* metadata_entry;
    const char* file_name[1] = {"resources/beowulf.txt"};
    ds3_client* client = get_client();
    const char* bucket_name = "metadata_test";
    FILE* file;

    printf("-----Testing put_metadata-------\n");

    ds3_request* request = ds3_init_put_bucket_request(bucket_name);

    error = ds3_put_bucket_request(client, request);
    ds3_request_free(request);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);

    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    ds3_request_set_custom_query_param(request, "job", bulk_response->job_id->value);
    file = fopen(obj_list->objects[0]->name->value, "r");
    ds3_bulk_object_list_response_free(obj_list);

    ds3_request_set_metadata(request, "name", "value");

    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_head_object_request(bucket_name, "resources/beowulf.txt");

    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);
    BOOST_REQUIRE(metadata_result != NULL);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 1);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "name");
    BOOST_REQUIRE(metadata_entry != NULL);
    BOOST_CHECK(g_strcmp0(metadata_entry->name->value, "name") == 0);

    ds3_metadata_entry_free(metadata_entry);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( head_bucket ) {
    printf("-----Testing head_bucket-------\n");
    ds3_error* error;
    ds3_client* client = get_client();
    const char* bucket_name = "metadata_test";
    ds3_request* request = ds3_init_put_bucket_request(bucket_name);

    error = ds3_put_bucket_request(client, request);
    ds3_request_free(request);
    handle_error(error);

    request = ds3_init_head_bucket_request(bucket_name);

    error = ds3_head_bucket_request(client, request);
    ds3_request_free(request);
    handle_error(error);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( head_folder ) {
    printf("-----Testing head_folder-------\n");
    ds3_metadata* metadata_result;
    ds3_error* error;
    ds3_client* client = get_client();
    const char* bucket_name = "head_folder_test";
    ds3_request* request = ds3_init_put_bucket_request(bucket_name);
    error = ds3_put_bucket_request(client, request);
    ds3_request_free(request);
    handle_error(error);

    const char* test_folder = "test_folder/";
    request = ds3_init_put_object_request(bucket_name, test_folder, 0);
    error   = ds3_put_object_request(client, request, NULL, NULL);
    ds3_request_free(request);
    handle_error(error);

    request = ds3_init_head_object_request(bucket_name, test_folder);

    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);
    BOOST_REQUIRE(metadata_result != NULL);
    ds3_metadata_free(metadata_result);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_multiple_metadata_items ) {
    ds3_error* error;
    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result;
    ds3_metadata_entry* metadata_entry;
    const char* file_name[1] = {"resources/beowulf.txt"};
    ds3_client* client = get_client_at_loglvl(DS3_DEBUG);
    const char* bucket_name = "multi_metadata_test";
    FILE* file;

    printf("-----Testing put_multiple_metadata_items-------\n");

    ds3_request* request = ds3_init_put_bucket_request(bucket_name);

    error = ds3_put_bucket_request(client, request);
    ds3_request_free(request);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    ds3_request_set_custom_query_param(request, "job", bulk_response->job_id->value);
    file = fopen(obj_list->objects[0]->name->value, "r");
    ds3_bulk_object_list_response_free(obj_list);

    ds3_request_set_metadata(request, "key", "value2");
    ds3_request_set_metadata(request, "name", "value");
    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_head_object_request(bucket_name, "resources/beowulf.txt");
    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);
    BOOST_REQUIRE(metadata_result != NULL);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 2);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "name");
    BOOST_REQUIRE(metadata_entry != NULL);
    BOOST_CHECK(g_strcmp0(metadata_entry->name->value, "name") == 0);
    BOOST_CHECK(g_strcmp0(metadata_entry->values[0]->value, "value") == 0);
    ds3_metadata_entry_free(metadata_entry);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "key");
    BOOST_REQUIRE(metadata_entry != NULL);
    BOOST_CHECK(g_strcmp0(metadata_entry->name->value, "key") == 0);
    BOOST_CHECK(g_strcmp0(metadata_entry->values[0]->value, "value2") == 0);

    ds3_metadata_entry_free(metadata_entry);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}

static bool contains_key(const ds3_metadata_keys_result* metadata_keys, const char* key) {
    uint64_t i;
    for (i = 0; i < metadata_keys->num_keys; i++) {
        if (g_strcmp0(key, metadata_keys->keys[i]->value) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOST_AUTO_TEST_CASE( metadata_keys ) {
    ds3_error* error;
    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result = NULL;
    ds3_metadata_keys_result* metadata_keys = NULL;

    const char* file_name[1] = {"resources/beowulf.txt"};
    ds3_client* client = get_client_at_loglvl(DS3_DEBUG);
    const char* bucket_name = "key_metadata_test";
    FILE* file;

    printf("-----Testing metadata_keys-------\n");

    ds3_request* request = ds3_init_put_bucket_request(bucket_name);

    error = ds3_put_bucket_request(client, request);
    ds3_request_free(request);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    ds3_request_set_custom_query_param(request, "job", bulk_response->job_id->value);
    file = fopen(obj_list->objects[0]->name->value, "r");
    ds3_bulk_object_list_response_free(obj_list);

    ds3_request_set_metadata(request, "key", "value2");
    ds3_request_set_metadata(request, "name", "value");

    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_head_object_request(bucket_name, "resources/beowulf.txt");

    error = ds3_head_object_request(client, request, &metadata_result);
    ds3_request_free(request);
    handle_error(error);
    BOOST_REQUIRE(metadata_result != NULL);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 2);

    metadata_keys = ds3_metadata_keys(metadata_result);
    BOOST_REQUIRE(metadata_keys != NULL);

    BOOST_CHECK(metadata_keys->num_keys == 2);
    BOOST_CHECK(contains_key(metadata_keys, "key"));
    BOOST_CHECK(contains_key(metadata_keys, "name"));

    ds3_metadata_keys_free(metadata_keys);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_metadata_using_get_object_retrieval ) {
    ds3_error* error;
    ds3_bulk_object_list_response* obj_list;
    uint64_t metadata_count;
    ds3_master_object_list_response* bulk_response;
    ds3_metadata* metadata_result;
    ds3_metadata_entry* metadata_entry;
    const char* file_name[1] = {"resources/beowulf.txt"};
    ds3_client* client = get_client();
    const char* bucket_name = "get_object_metadata_test";
    FILE* file;

    printf("-----Testing put_metadata_using_get_object_retrieval-------\n");

    ds3_request* request = ds3_init_put_bucket_request(bucket_name);

    error = ds3_put_bucket_request(client, request);
    ds3_request_free(request);
    handle_error(error);

    obj_list = ds3_convert_file_list(file_name, 1);

    request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    error = ds3_put_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    handle_error(error);

    request = ds3_init_put_object_request(bucket_name, "resources/beowulf.txt", obj_list->objects[0]->length);
    file = fopen(obj_list->objects[0]->name->value, "r");

    ds3_request_set_metadata(request, "name", "value");
    error = ds3_put_object_request(client, request, file, ds3_read_from_file);
    ds3_request_free(request);
    fclose(file);
    handle_error(error);
    ds3_master_object_list_response_free(bulk_response);

    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, obj_list);
    ds3_request_set_custom_query_param(request, "chunkClientProcessingOrderGuarantee", "NONE");
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(obj_list);
    handle_error(error);

    request = ds3_init_get_object_request(bucket_name, "resources/beowulf.txt", 0);
    ds3_request_set_custom_query_param(request, "job", bulk_response->job_id->value);
    file = fopen("/dev/null", "w");
    error = ds3_get_object_with_metadata(client, request, file, ds3_write_to_file, &metadata_result);
    ds3_request_free(request);
    ds3_master_object_list_response_free(bulk_response);
    fclose(file);
    handle_error(error);
    BOOST_REQUIRE(metadata_result != NULL);

    metadata_count = ds3_metadata_size(metadata_result);
    BOOST_CHECK(metadata_count == 1);

    metadata_entry = ds3_metadata_get_entry(metadata_result, "name");
    BOOST_REQUIRE(metadata_entry != NULL);
    BOOST_CHECK(g_strcmp0(metadata_entry->name->value, "name") == 0);

    ds3_metadata_entry_free(metadata_entry);
    ds3_metadata_free(metadata_result);
    clear_bucket(client, bucket_name);
    free_client(client);
}
