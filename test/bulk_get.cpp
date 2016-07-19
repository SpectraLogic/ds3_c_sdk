#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ds3.h"
#include "ds3_net.h"
#include "test.h"
#include <boost/test/unit_test.hpp>
#include "checksum.h"

static const unsigned char MAX_UNIT_TEST_FILEPATH_LENGTH = 64;

static const char FOLDER_PREFIX[] = "resources/";
static const unsigned char LENGTH_OF_FOLDER_PREFIX = sizeof(FOLDER_PREFIX);
static const char TEMP_PREFIX[] = "temp-";
static const unsigned char LENGTH_OF_TEMP_PREFIX = sizeof(TEMP_PREFIX);

struct checksum_result {
    char original_name[MAX_UNIT_TEST_FILEPATH_LENGTH];
    char tmp_name[MAX_UNIT_TEST_FILEPATH_LENGTH];
    int num_chunks;
    bool passed;
};

bool check_all_passed(uint32_t num_files, checksum_result* results) {
    bool result = true;
    for (uint32_t i = 0; i < num_files; i++) {
        result = result && results[i].passed;
    }
    return result;
}

uint32_t get_number_of_chunks(uint32_t num_files, checksum_result* results, const char* filename) {
    for (uint32_t i = 0; i < num_files; i++) {
        if (g_strcmp0(results[i].original_name, filename) == 0) {
            return results[i].num_chunks;
        }
    }
    return 0;
}

uint32_t get_sum_of_chunks(uint32_t num_files, checksum_result* results) {
    uint32_t result = 0;
    for (uint32_t i = 0; i < num_files; i++) {
        result += results[i].num_chunks;
    }
    return result;
}

uint32_t getFileIndexForChunk(uint64_t* max_file_index, ds3_str* current_obj_name, checksum_result* results) {
    printf("getFileIndexForChunk\n");
    printf("  curr_obj_name[%s]\n", current_obj_name->value);
    printf("max_file_index: %lu\n", *max_file_index);
    int64_t file_index = -1;
    for (uint64_t current_file_index = 0; current_file_index < *max_file_index; current_file_index++) {
        if (g_strcmp0(current_obj_name->value, results[current_file_index].original_name) == 0) {
            file_index = current_file_index;
            break;
        }
    }

    printf("file_index: %lu\n", file_index);
    if (file_index == -1) {

        file_index = *max_file_index;
        printf("  file_index: %lu\n", file_index);
        (*max_file_index)++;
        printf("  max_file_index: %lu\n", *max_file_index);

        memcpy(results[file_index].original_name, current_obj_name->value, current_obj_name->size);

        memcpy(results[file_index].tmp_name, FOLDER_PREFIX, LENGTH_OF_FOLDER_PREFIX); // "resources/"
        memcpy(results[file_index].tmp_name+LENGTH_OF_FOLDER_PREFIX-1, TEMP_PREFIX, LENGTH_OF_TEMP_PREFIX); // "resources/temp-"
        memcpy(results[file_index].tmp_name+LENGTH_OF_FOLDER_PREFIX+LENGTH_OF_TEMP_PREFIX-2, current_obj_name->value+LENGTH_OF_FOLDER_PREFIX-1, current_obj_name->size-LENGTH_OF_FOLDER_PREFIX+1);
        printf("  tmp_name[%s]\n", results[file_index].tmp_name);
    }
    results[file_index].num_chunks++;
    return file_index;
}

void checkChunkResponse(ds3_client* client, uint32_t num_files, ds3_master_object_list_response* chunk_response, checksum_result* results) {
    uint64_t max_file_index = 0;
    int64_t file_index = 0;
    ds3_request* request = NULL;
    ds3_error* error = NULL;

    for (uint64_t chunk_index = 0; chunk_index < chunk_response->num_objects; chunk_index++) {
        ds3_objects_response* chunk_object_list = chunk_response->objects[chunk_index];
        for (uint64_t chunk_object_index = 0; chunk_object_index < chunk_object_list->num_objects; chunk_object_index++) {
            FILE* w_file;
            ds3_bulk_object_response* current_obj = chunk_object_list->objects[chunk_object_index];
            printf("checkChunkResponse: obj %s in bucket %s\n", current_obj->name->value, chunk_response->bucket_name->value);
            file_index = getFileIndexForChunk(&max_file_index, current_obj->name, results);

            const uint64_t length = current_obj->length;
            printf("checkChunkResponse length: %lu\n", length);
            const uint64_t offset = current_obj->offset;
            printf("checkChunkResponse offset: %lu\n", offset);
            request = ds3_init_get_object_request(chunk_response->bucket_name->value, current_obj->name->value, &length, chunk_response->job_id->value, &offset);

            w_file = fopen(results[file_index].tmp_name, "a+");
            fseek(w_file, current_obj->offset, SEEK_SET);

            error = ds3_get_object_request(client, request, w_file, ds3_write_to_file);
            ds3_request_free(request);
            fclose(w_file);
            handle_error(error);
        }
    }

    for (uint64_t current_file_index = 0; current_file_index < max_file_index; current_file_index++) {
        printf("------Performing Data Integrity Test-------\n");
        results[current_file_index].passed = compare_hash(results[current_file_index].original_name, results[current_file_index].tmp_name);
        unlink(results[current_file_index].tmp_name);
    }
}

void checkChunkResponsePartials(ds3_client* client, uint32_t num_files, ds3_master_object_list_response* chunk_response, checksum_result* results, uint32_t segment_size) {
    uint64_t max_file_index = 0;
    int64_t file_index = 0;
    ds3_request* request = NULL;
    ds3_error* error = NULL;

    for (uint64_t chunk_index = 0; chunk_index < chunk_response->num_objects; chunk_index++) {
        ds3_objects_response* chunk_object_list = chunk_response->objects[chunk_index];
        for (uint64_t chunk_object_index = 0; chunk_object_index < chunk_object_list->num_objects; chunk_object_index++) {
            FILE* w_file;
            ds3_bulk_object_response* current_obj = chunk_object_list->objects[chunk_object_index];
            file_index = getFileIndexForChunk(&max_file_index, current_obj->name, results);

            const uint64_t length = current_obj->length;
            const uint64_t offset = current_obj->offset;
            request = ds3_init_get_object_request(chunk_response->bucket_name->value, current_obj->name->value, &length, chunk_response->job_id->value, &offset);

            ds3_request_set_byte_range(request, segment_size, segment_size*2-1);
            ds3_request_set_byte_range(request, segment_size*3, segment_size*4-1);

            w_file = fopen(results[file_index].tmp_name, "a+");
            fseek(w_file, current_obj->offset, SEEK_SET);

            error = ds3_get_object_request(client, request, w_file, ds3_write_to_file);
            ds3_request_free(request);
            fclose(w_file);
            handle_error(error);
        }
    }

    for (uint64_t current_file_index = 0; current_file_index < max_file_index; current_file_index++) {
        printf("------Performing Data Integrity Test-------\n");
        bool check1 = compare_hash_extended(results[current_file_index].original_name, results[current_file_index].tmp_name, segment_size, segment_size, 0);
        bool check2 = compare_hash_extended(results[current_file_index].original_name, results[current_file_index].tmp_name, segment_size, segment_size*3, segment_size);
        results[current_file_index].passed = check1 && check2;
        unlink(results[current_file_index].tmp_name);
    }
}

BOOST_AUTO_TEST_CASE( bulk_get ) {
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    const uint32_t num_files = 5;
    ds3_bulk_object_list_response* object_list = default_object_list();
    ds3_master_object_list_response* chunk_response = NULL;

    ds3_client* client = get_client();
    const char* bucket_name = "test_bulk_get_bucket";

    printf("-----Testing Bulk GET-------\n");

    populate_with_objects(client, bucket_name);

    ds3_job_chunk_client_processing_order_guarantee chunk_order = DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE;
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, NULL, &chunk_order, NULL, NULL, object_list);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);
    BOOST_CHECK(handle_error_and_return_is_null(error));

    chunk_response = ensure_available_chunks(client, bulk_response->job_id);
    BOOST_REQUIRE(handle_error_and_return_is_null(error));

    checksum_result* checksum_results = (checksum_result*) calloc(num_files, sizeof(checksum_result));
    checkChunkResponse(client, num_files, chunk_response, checksum_results);
    BOOST_CHECK(check_all_passed(num_files, checksum_results) == true);
    BOOST_CHECK(get_sum_of_chunks(num_files, checksum_results) == num_files);
    free(checksum_results);

    // check to make sure that the 'job' has completed
    /* works sporadically in 1.2, shjould be fixed in 3.0
    ds3_bulk_response* completed_job = NULL;
    request = ds3_init_get_job(bulk_response->job_id->value);
    error = ds3_get_job(client, request, &completed_job);
    handle_error(error);

    BOOST_CHECK(completed_job != NULL);
    BOOST_CHECK(completed_job->status == COMPLETED);

    ds3_free_request(request);
    ds3_free_bulk_response(completed_job);*/

    ds3_master_object_list_response_free(chunk_response);
    ds3_master_object_list_response_free(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( max_upload_size ) {
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_bulk_object_list_response* object_list = NULL;
    ds3_master_object_list_response* chunk_response = NULL;

    ds3_client* client = get_client();
    const char* bucket_name = "test_max_upload_size_bucket";

    printf("-----Testing Bulk GET with max_upload_size-------\n");

    const uint32_t num_files = 6;
    const char* books[num_files] = {"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/tale_of_two_cities.txt", "resources/ulysses.txt", "resources/ulysses_large.txt", "resources/ulysses_46mb.txt"};
    object_list = ds3_convert_file_list(books, num_files);

    request = populate_bulk_return_request(client, bucket_name, object_list);
    ds3_request_set_max_upload_size(request, 10485760);
    bulk_response = populate_bulk_return_response(client, request);
    populate_with_objects_from_bulk(client, bucket_name, bulk_response);
    ds3_master_object_list_response_free(bulk_response);

    ds3_job_chunk_client_processing_order_guarantee chunk_order = DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE;
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, NULL, &chunk_order, NULL, NULL, object_list);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);

    BOOST_REQUIRE(handle_error_and_return_is_null(error));

    chunk_response = ensure_available_chunks(client, bulk_response->job_id);

    BOOST_REQUIRE(handle_error_and_return_is_null(error));

    checksum_result* checksum_results = (checksum_result*) calloc(num_files, sizeof(checksum_result));
    checkChunkResponse(client, num_files, chunk_response, checksum_results);

    BOOST_CHECK(check_all_passed(num_files, checksum_results) == true);
    BOOST_CHECK(get_number_of_chunks(num_files, checksum_results, "resources/ulysses_large.txt") == 2);
    BOOST_CHECK(get_number_of_chunks(num_files, checksum_results, "resources/ulysses_46mb.txt") == 5);
    BOOST_CHECK(get_sum_of_chunks(num_files, checksum_results) == 11);

    free(checksum_results);

    // check to make sure that the 'job' has completed
    /* works sporadically in 1.2, shjould be fixed in 3.0
    ds3_bulk_response* completed_job = NULL;
    request = ds3_init_get_job(bulk_response->job_id->value);
    error = ds3_get_job(client, request, &completed_job);

    handle_error(error);

    BOOST_CHECK(completed_job != NULL);
    BOOST_CHECK(completed_job->status == COMPLETED);

    ds3_free_request(request);
    ds3_free_bulk_response(completed_job);*/

    ds3_master_object_list_response_free(chunk_response);
    ds3_master_object_list_response_free(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}


BOOST_AUTO_TEST_CASE( chunk_preference ) {
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    const uint32_t num_files = 5;
    ds3_bulk_object_list_response* object_list = default_object_list();
    ds3_master_object_list_response* chunk_response = NULL;
    bool retry_get;

    ds3_client* client = get_client_at_loglvl(DS3_TRACE);
    const char* bucket_name = "test_chunk_preference_bucket";

    printf("-----Testing Bulk GET with chunk_preference-------\n");

    populate_with_objects(client, bucket_name);

    ds3_job_chunk_client_processing_order_guarantee chunk_order = DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE;
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, NULL, &chunk_order, NULL, NULL, object_list);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);
    BOOST_REQUIRE(handle_error_and_return_is_null(error));

    do {
        retry_get = false;
        request =  ds3_init_get_job_chunks_ready_for_client_processing_spectra_s3_request(bulk_response->job_id->value, NULL);
        error = ds3_get_job_chunks_ready_for_client_processing_spectra_s3_request(client, request, &chunk_response);
        ds3_request_free(request);

        BOOST_REQUIRE(handle_error_and_return_is_null(error));
        BOOST_REQUIRE(chunk_response != NULL);

        if (chunk_response->num_objects == 0) {
            // if this happens we need to try the request
            BOOST_TEST_MESSAGE( "Hit retry, sleeping for 30 seconds..."); //<< chunk_response->retry_after);
            retry_get = true;
            //TODO parse metadata retry_after
            //sleep(chunk_response->retry_after);
            sleep(30);
            ds3_master_object_list_response_free(chunk_response);
        }
    } while(retry_get);

    BOOST_REQUIRE(handle_error_and_return_is_null(error));

    checksum_result* checksum_results = (checksum_result*) calloc(num_files, sizeof(checksum_result));
    checkChunkResponse(client, num_files, chunk_response, checksum_results);

    BOOST_CHECK(check_all_passed(num_files, checksum_results) == true);
    BOOST_CHECK(get_sum_of_chunks(num_files, checksum_results) == num_files);

    free(checksum_results);

    // check to make sure that the 'job' has completed
    /* works sporadically in 1.2, shjould be fixed in 3.0
    ds3_bulk_response* completed_job = NULL;
    request = ds3_init_get_job(bulk_response->job_id->value);
    error = ds3_get_job(client, request, &completed_job);

    handle_error(error);

    BOOST_CHECK(completed_job != NULL);
    BOOST_CHECK(completed_job->status == COMPLETED);

    ds3_free_request(request);
    ds3_free_bulk_response(completed_job);*/

    ds3_master_object_list_response_free(chunk_response);
    ds3_master_object_list_response_free(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( partial_get ) {
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    const uint32_t num_files = 5;
    ds3_bulk_object_list_response* object_list = default_object_list();
    ds3_master_object_list_response* chunk_response = NULL;

    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";

    printf("-----Testing Bulk GET with partial range-------\n");

    populate_with_objects(client, bucket_name);

    ds3_job_chunk_client_processing_order_guarantee chunk_order = DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE;
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, NULL, &chunk_order, NULL, NULL, object_list);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);
    BOOST_REQUIRE(handle_error_and_return_is_null(error));

    chunk_response = ensure_available_chunks(client, bulk_response->job_id);
    BOOST_REQUIRE(handle_error_and_return_is_null(error));

    checksum_result* checksum_results = (checksum_result*) calloc(num_files, sizeof(checksum_result));
    checkChunkResponsePartials(client, num_files, chunk_response, checksum_results, 3200);

    BOOST_CHECK(check_all_passed(num_files, checksum_results) == true);
    BOOST_CHECK(get_sum_of_chunks(num_files, checksum_results) == num_files);

    free(checksum_results);

    // check to make sure that the 'job' has completed
    /* works sporadically in 1.2, shjould be fixed in 3.0
    ds3_bulk_response* completed_job = NULL;
    request = ds3_init_get_job(bulk_response->job_id->value);
    error = ds3_get_job(client, request, &completed_job);

    handle_error(error);

    BOOST_CHECK(completed_job != NULL);
    BOOST_CHECK(completed_job->status == COMPLETED);

    ds3_free_request(request);
    ds3_free_bulk_response(completed_job);*/

    ds3_master_object_list_response_free(chunk_response);
    ds3_master_object_list_response_free(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( escape_urls ) {
    const char *delimiters[4] = {"or", "/", "@", "="};
    const char *strings_to_test[5] = {"some normal text", "/an/object/name", "bytes=0-255,300-400,550-800", "orqwerty/qwerty@qwerty=", "`1234567890-=~!@#$%^&*()_+[]\{}|;:,./<>?"}; 
    const char *object_name_results[5] = {"some%20normal%20text", "/an/object/name", "bytes%3D0-255%2C300-400%2C550-800", "orqwerty/qwerty%40qwerty%3D",
                                        "%601234567890-%3D~%21%40%23%24%25%5E%26%2A%28%29_+%5B%5D%7B%7D%7C%3B%3A%2C./%3C%3E%3F"};
    const char *range_header_results[5] = {"some%20normal%20text", "%2Fan%2Fobject%2Fname", "bytes=0-255,300-400,550-800", "orqwerty%2Fqwerty%40qwerty=",
                                         "%601234567890-=~%21%40%23%24%25%5E%26%2A%28%29_%2B%5B%5D%7B%7D%7C%3B%3A,.%2F%3C%3E%3F"};
    const char *general_delimiter_results[5] = {"some%20normal%20text", "/an/object/name", "bytes=0-255%2C300-400%2C550-800", "orqwerty/qwerty@qwerty=",
                                              "%601234567890-=~%21@%23%24%25%5E%26%2A%28%29_%2B%5B%5D%7B%7D%7C%3B%3A%2C./%3C%3E%3F"};

    printf("-----Testing escape url helpers-------\n");

    for (int i = 0; i < 5; i++) {
        char* escaped_url = escape_url_object_name(strings_to_test[i]);
        BOOST_CHECK(strcmp(escaped_url, object_name_results[i]) == 0);
        free(escaped_url);
    }
    for (int i = 0; i < 5; i++) {
        char* escaped_url = escape_url_range_header(strings_to_test[i]);
        BOOST_CHECK(strcmp(escaped_url, range_header_results[i]) == 0);
        free(escaped_url);
    }
    for (int i = 0; i < 5; i++) {
        char* escaped_url = escape_url_extended(strings_to_test[i], delimiters, 4);
        BOOST_CHECK(strcmp(escaped_url, general_delimiter_results[i]) == 0);
        free(escaped_url);
    }
}

BOOST_AUTO_TEST_CASE( convert_list_helper ) {
    const char* books[4] = {"beowulf.txt", "sherlock_holmes.txt", "tale_of_two_cities.txt", "ulysses.txt"};
    ds3_bulk_object_list_response* obj_list;

    printf("-----Testing convert_list helper-------\n");

    obj_list = ds3_convert_file_list_with_basepath(books, 4, "resources/");

    BOOST_CHECK(strcmp(obj_list->objects[0]->name->value, "beowulf.txt") == 0);
    BOOST_CHECK(obj_list->objects[0]->length == 294059);

    ds3_bulk_object_list_response_free(obj_list);
}

BOOST_AUTO_TEST_CASE( directory_size ) {
    const char* books[1] = {"resources"};
    ds3_bulk_object_list_response* obj_list;

    printf("-----Testing directory size on convert list helper-------\n");

    obj_list = ds3_convert_file_list_with_basepath(books, 1, NULL);

    BOOST_CHECK(strcmp(obj_list->objects[0]->name->value, "resources") == 0);
    BOOST_CHECK(obj_list->objects[0]->length == 0);

    ds3_bulk_object_list_response_free(obj_list);
}
