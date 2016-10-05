// The BOOST_TEST_MODULE should be set only in this file
// so that a main function is only generated once
#define BOOST_TEST_MODULE DS3_Tests
#include <stdio.h>
#include <stdlib.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/included/unit_test.hpp>

TempStorageIds ids;

struct BoostTestFixture {
    BoostTestFixture() {
        configure_for_tests();
    }
    ~BoostTestFixture() {
        teardown_after_tests();

        ds3_str_free(ids.data_policy_id);
        ds3_str_free(ids.data_persistence_rule_id);
        ds3_str_free(ids.storage_domain_member_id);

        ds3_cleanup();
    }
};

BOOST_GLOBAL_FIXTURE( BoostTestFixture );

void test_log(const char* message, void* user_data) {
    fprintf(stderr, "Log Message: %s\n", message);
}

ds3_client* get_client_at_loglvl(ds3_log_lvl log_lvl) {
    ds3_client* client;

    ds3_error* error = ds3_create_client_from_env(&client);

    if (error != NULL) {
        fprintf(stderr, "Failed to construct ds3_client from environment variables: %s\n", error->message->value);
        ds3_error_free(error);
        BOOST_FAIL("Failed to setup client.");
    }

    ds3_client_register_logging(client, log_lvl, test_log, NULL);

    return client;
}

ds3_client* get_client() {
    return get_client_at_loglvl(DS3_INFO);
}

void free_client(ds3_client* client) {
    ds3_creds_free(client->creds);
    ds3_client_free(client);
}

void configure_for_tests() {
    ds3_client* client = get_client();

    ds3_data_policy_response* put_dp_response = NULL;
    ds3_storage_domain_response* put_sd_response = NULL;
    ds3_pool_partition_response* put_pp_response = NULL;
    ds3_storage_domain_member_response* put_sd_member_response = NULL;
    ds3_data_persistence_rule_response* put_data_persistence_rule_response = NULL;

    ds3_str* data_policy_id;
    ds3_str* storage_domain_id;
    ds3_str* pool_partition_id;
    ds3_str* storage_domain_member_id;
    ds3_str* data_persistence_rule_id;

    // Create DataPolicy
    ds3_request* put_dp_request = ds3_init_put_data_policy_spectra_s3_request(TEST_DP_NAME);
    ds3_error* error = ds3_put_data_policy_spectra_s3_request(client, put_dp_request, &put_dp_response);
    data_policy_id = ds3_str_init(put_dp_response->id->value);

    ds3_request_free(put_dp_request);
    ds3_data_policy_response_free(put_dp_response);
    ds3_error_free(error);

    // Create StorageDomain
    ds3_request* put_sd_request = ds3_init_put_storage_domain_spectra_s3_request(TEST_SD_NAME);
    error = ds3_put_storage_domain_spectra_s3_request(client, put_sd_request, &put_sd_response);
    storage_domain_id = ds3_str_init(put_sd_response->id->value);

    ds3_request_free(put_sd_request);
    ds3_storage_domain_response_free(put_sd_response);
    ds3_error_free(error);

    // Create pool partition
    ds3_request* put_pp_request = ds3_init_put_pool_partition_spectra_s3_request(TEST_PP_NAME, DS3_POOL_TYPE_ONLINE);
    error = ds3_put_pool_partition_spectra_s3_request(client, put_pp_request, &put_pp_response);
    pool_partition_id = ds3_str_init(put_pp_response->id->value);

    ds3_request_free(put_pp_request);
    ds3_pool_partition_response_free(put_pp_response);
    ds3_error_free(error);

    // Create storage domain member linking pool partition to storage domain
    ds3_request* put_pool_sd_member_request = ds3_init_put_pool_storage_domain_member_spectra_s3_request(pool_partition_id->value, storage_domain_id->value);
    error = ds3_put_pool_storage_domain_member_spectra_s3_request(client, put_pool_sd_member_request, &put_sd_member_response);
    storage_domain_member_id = ds3_str_init(put_sd_member_response->id->value);

    ds3_request_free(put_pool_sd_member_request);
    ds3_storage_domain_member_response_free(put_sd_member_response);
    ds3_str_free(pool_partition_id);
    ds3_error_free(error);

    // Create data persistence rule
    ds3_request* put_data_persistence_rule_request = ds3_init_put_data_persistence_rule_spectra_s3_request(data_policy_id->value,
        DS3_DATA_ISOLATION_LEVEL_STANDARD,
        storage_domain_id->value,
        DS3_DATA_PERSISTENCE_RULE_TYPE_PERMANENT);
    error = ds3_put_data_persistence_rule_spectra_s3_request(client, put_data_persistence_rule_request, &put_data_persistence_rule_response);
    data_persistence_rule_id = ds3_str_init(put_data_persistence_rule_response->id->value);

    ds3_request_free(put_data_persistence_rule_request);
    ds3_data_persistence_rule_response_free(put_data_persistence_rule_response);
    ds3_str_free(storage_domain_id);
    ds3_error_free(error);

    free_client(client);

    ids.data_policy_id = data_policy_id;
    ids.data_persistence_rule_id = data_persistence_rule_id;
    ids.storage_domain_member_id = storage_domain_member_id;
}

void teardown_after_tests() {
    ds3_client* client = get_client();
    ds3_error* error;

    // Delete DataPersistenceRule
    ds3_request* delete_data_persistence_rule_request = ds3_init_delete_data_persistence_rule_spectra_s3_request(ids.data_persistence_rule_id->value);
    error = ds3_delete_data_persistence_rule_spectra_s3_request(client, delete_data_persistence_rule_request);
    ds3_request_free(delete_data_persistence_rule_request);
    ds3_error_free(error);

    // Delete DataPolicy
    ds3_request* delete_data_policy_request = ds3_init_delete_data_policy_spectra_s3_request(TEST_DP_NAME);
    error = ds3_delete_data_policy_spectra_s3_request(client, delete_data_policy_request);
    ds3_request_free(delete_data_policy_request);
    ds3_error_free(error);

    // Delete StorageDomainMember
    ds3_request* delete_storage_domain_member_request = ds3_init_delete_storage_domain_member_spectra_s3_request(ids.storage_domain_member_id->value);
    error = ds3_delete_storage_domain_member_spectra_s3_request(client, delete_storage_domain_member_request);
    ds3_request_free(delete_storage_domain_member_request);
    ds3_error_free(error);

    // Delete StorageDomain
    ds3_request* delete_storage_domain_request = ds3_init_delete_storage_domain_spectra_s3_request(TEST_SD_NAME);
    error = ds3_delete_storage_domain_spectra_s3_request(client, delete_storage_domain_request);
    ds3_request_free(delete_storage_domain_request);
    ds3_error_free(error);

    // Delete PoolPartition
    ds3_request* delete_pool_partition_request = ds3_init_delete_pool_partition_spectra_s3_request(TEST_PP_NAME);
    error = ds3_delete_pool_partition_spectra_s3_request(client, delete_pool_partition_request);
    ds3_request_free(delete_pool_partition_request);
    ds3_error_free(error);

    free_client(client);
}

void print_error(const ds3_error* error) {
      if (NULL != error->message) {
        printf("ds3_error_message: %s\n", error->message->value);
      }
      if (error->error != NULL) {
          printf("ds3_status_code: %d\n", error->error->http_error_code);
          printf("ds3_status_message: %s\n", error->error->message->value);
          printf("ds3_error_body: %s\n", error->error->code->value);
      }
}

void handle_error(ds3_error* error) {
    BOOST_CHECK(error == NULL);
    if (error != NULL) {
        print_error(error);
        ds3_error_free(error);
    }
}

void clear_bucket(const ds3_client* client, const char* bucket_name) {
    ds3_request* request;
    ds3_error* error;

    request = ds3_init_delete_bucket_spectra_s3_request(bucket_name);
    ds3_request_set_force(request, True);

    error = ds3_delete_bucket_spectra_s3_request(client, request);
    ds3_request_free(request);
    ds3_error_free(error);
}

void populate_with_objects(const ds3_client* client, const char* bucket_name) {
    ds3_str* job_id = populate_with_objects_return_job(client, bucket_name);
    ds3_str_free(job_id);
}

ds3_bulk_object_list_response* default_object_list() {
    const char* books[5] = {"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/tale_of_two_cities.txt", "resources/ulysses.txt", "resources/ulysses_large.txt"};
    return ds3_convert_file_list(books, 5);
}

ds3_request* populate_bulk_return_request(const ds3_client* client, const char* bucket_name, ds3_bulk_object_list_response* obj_list) {
    ds3_error* error = create_bucket_with_data_policy(client, bucket_name, ids.data_policy_id->value);
    ds3_error_free(error);

    ds3_request* request = ds3_init_put_bulk_job_spectra_s3_request(bucket_name, obj_list);
    return request;
}

ds3_master_object_list_response* populate_bulk_return_response(const ds3_client* client, ds3_request* request) {
    ds3_master_object_list_response* response;
    ds3_error* error = ds3_put_bulk_job_spectra_s3_request(client, request, &response);

    ds3_request_free(request);
    handle_error(error);
    return response;
}

ds3_str* populate_with_empty_objects(const ds3_client* client, const char* bucket_name) {
    ds3_bulk_object_list_response* obj_list = default_object_list();
    ds3_request* request = populate_bulk_return_request(client, bucket_name, obj_list);
    ds3_master_object_list_response* response = populate_bulk_return_response(client, request);

    ds3_str* job_id = ds3_str_dup(response->job_id);
    ds3_master_object_list_response_free(response);
    ds3_bulk_object_list_response_free(obj_list);
    return job_id;
}

ds3_master_object_list_response* ensure_available_chunks(const ds3_client* client, ds3_str* job_id) {
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    bool retry_get;
    ds3_master_object_list_response* chunk_response;
    do {
        retry_get = false;
        request =  ds3_init_get_job_chunks_ready_for_client_processing_spectra_s3_request(job_id->value);
        error = ds3_get_job_chunks_ready_for_client_processing_spectra_s3_request(client, request, &chunk_response);
        ds3_request_free(request);

        handle_error(error);
        BOOST_CHECK(chunk_response != NULL);

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
    return chunk_response;
}

void populate_with_objects_from_bulk(const ds3_client* client, const char* bucket_name, ds3_master_object_list_response* response) {
    uint64_t chunk_index, object_index;
    ds3_error* error;
    ds3_request* request;

    ds3_master_object_list_response* chunk_response = ensure_available_chunks(client, response->job_id);

    for (chunk_index = 0; chunk_index < chunk_response->num_objects; chunk_index++) {
        ds3_objects_response* chunk_object_list = chunk_response->objects[chunk_index];
        for (object_index = 0; object_index < chunk_object_list->num_objects; object_index++) {
            ds3_bulk_object_response* current_obj = chunk_object_list->objects[object_index];
            FILE* file = fopen(current_obj->name->value, "r");

            request = ds3_init_put_object_request(bucket_name, current_obj->name->value, current_obj->length);
            ds3_request_set_job(request, response->job_id->value);
            ds3_request_set_offset(request, current_obj->offset);
            if (current_obj->offset > 0) {
                fseek(file, current_obj->offset, SEEK_SET);
            }
            error = ds3_put_object_request(client, request, file, ds3_read_from_file);
            ds3_request_free(request);

            fclose(file);
            handle_error(error);
        }
    }
    ds3_master_object_list_response_free(chunk_response);
}

ds3_str* populate_with_objects_return_job(const ds3_client* client, const char* bucket_name) {
    ds3_bulk_object_list_response* bulk_object_list_response = default_object_list();
    ds3_request* request = populate_bulk_return_request(client, bucket_name, bulk_object_list_response);
    ds3_master_object_list_response* master_object_list_response = populate_bulk_return_response(client, request);

    ds3_str* job_id = ds3_str_dup(master_object_list_response->job_id);
    populate_with_objects_from_bulk(client, bucket_name, master_object_list_response);

    ds3_bulk_object_list_response_free(bulk_object_list_response);
    ds3_master_object_list_response_free(master_object_list_response);
    return job_id;
}

bool contains_object(ds3_list_bucket_result_response* bucket_list, const char* key) {
    uint64_t object_index;
    for (object_index = 0; object_index < bucket_list->num_objects; object_index++) {
        if (strcmp(bucket_list->objects[object_index]->key->value, key) == 0) {
            return true;
        }
    }
    return false;
}

ds3_error* create_bucket_with_data_policy(const ds3_client* client, const char* bucket_id, const char* data_policy_id) {
    ds3_request* request = ds3_init_put_bucket_spectra_s3_request(bucket_id);
    ds3_request_set_data_policy_id(request, data_policy_id);

    ds3_bucket_response* bucket_response = NULL;
    ds3_error* error = ds3_put_bucket_spectra_s3_request(client, request, &bucket_response);
    ds3_bucket_response_free(bucket_response);
    ds3_request_free(request);
    return error;
}

ds3_error* get_bucket_data_policy_id(const ds3_client* client, const char* bucket_name, ds3_str* data_policy_id) {
    ds3_request* request;
    ds3_error* error;
    ds3_bucket_response* bucket_response;

    request = ds3_init_get_bucket_spectra_s3_request(bucket_name);
    error = ds3_get_bucket_spectra_s3_request(client, request, &bucket_response);
    ds3_request_free(request);
    if (error != NULL) {
        ds3_bucket_response_free(bucket_response);
        return error;
    }

    data_policy_id = bucket_response->data_policy_id;
    ds3_bucket_response_free(bucket_response);

    return NULL;
}

ds3_error* get_bucket_data_policy_checksum_type(ds3_client* client, const char* bucket_name, ds3_checksum_type* checksum_type) {
    ds3_request* request;
    ds3_error* error;
    ds3_str* data_policy_id = NULL;
    ds3_data_policy_response* data_policy_response;

    // Get bucket data policy ID
    error = get_bucket_data_policy_id(client, bucket_name, data_policy_id);
    if (error != NULL) {
        ds3_data_policy_response_free(data_policy_response);
        return error;
    }

    // Get DataPolicy checksum_type
    request = ds3_init_get_data_policy_spectra_s3_request(data_policy_id->value);
    error = ds3_get_data_policy_spectra_s3_request(client, request, &data_policy_response);
    ds3_request_free(request);
    ds3_str_free(data_policy_id);
    if (error != NULL) {
        ds3_data_policy_response_free(data_policy_response);
        return error;
    }

    *checksum_type = data_policy_response->checksum_type;

    // cleanup
    ds3_data_policy_response_free(data_policy_response);

    return NULL;
}
