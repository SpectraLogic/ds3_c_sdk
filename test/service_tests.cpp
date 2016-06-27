#include <stdbool.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( get_service_no_buckets ) {
    ds3_client* client = get_client();
    ds3_request* request = ds3_init_get_service_request();
    ds3_list_all_my_buckets_result_response* response;

    printf("-----Testing GET service-------\n");

    ds3_error* error = ds3_get_service_request(client, request, &response);
    BOOST_CHECK(error == NULL);

    ds3_request_free(request);
    ds3_list_all_my_buckets_result_response_free(response);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_service_one_bucket ) {
    printf("-----Testing GET service after PUT bucket-------\n");

    ds3_client* client = get_client();
    uint64_t bucket_index;
    bool found = false;
    const char* bucket_name = "test_put_bucket";
    ds3_request* request = ds3_init_put_bucket_spectra_s3_request(bucket_name);
    ds3_request_set_data_policy_id(request, ids.data_policy_id->value);
    ds3_bucket_response* bucket_response;
    ds3_list_all_my_buckets_result_response* service_response;

    ds3_error* error = ds3_put_bucket_spectra_s3_request(client, request, &bucket_response);
    BOOST_CHECK(error == NULL);
    ds3_bucket_response_free(bucket_response);
    ds3_request_free(request);
    ds3_error_free(error);

    request = ds3_init_get_service_request();
    error = ds3_get_service_request(client, request, &service_response);
    BOOST_CHECK(error == NULL);
    ds3_request_free(request);
    ds3_error_free(error);

    for (bucket_index = 0; bucket_index < service_response->num_buckets; bucket_index++) {
        fprintf(stderr, "Expected Name (%s) actual (%s)\n", bucket_name, service_response->buckets[bucket_index]->name->value);
        if (strcmp(bucket_name, service_response->buckets[bucket_index]->name->value) == 0) {
            found = true;
            break;
        }
    }
    BOOST_CHECK(found);
    ds3_list_all_my_buckets_result_response_free(service_response);

    request = ds3_init_delete_bucket_request(bucket_name);
    error = ds3_delete_bucket_request(client, request);
    BOOST_CHECK(error == NULL);
    ds3_request_free(request);
    ds3_error_free(error);

    free_client(client);
}

BOOST_AUTO_TEST_CASE( get_system_information ) {
    ds3_client* client = get_client();
    ds3_request* request = ds3_init_get_system_information_spectra_s3_request();
    ds3_system_information_response* response;

    printf("-----Testing GET system_information-------\n");

    ds3_error* error = ds3_get_system_information_spectra_s3_request(client, request, &response);
    BOOST_CHECK(error == NULL);

    BOOST_CHECK(response->api_version != NULL);
    BOOST_CHECK(response->serial_number != NULL);

    BOOST_CHECK(response->build_information != NULL);
    BOOST_CHECK(response->build_information->branch != NULL);
    BOOST_CHECK(response->build_information->revision != NULL);
    BOOST_CHECK(response->build_information->version != NULL);

    ds3_request_free(request);
    ds3_system_information_response_free(response);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( verify_system_health ) {
    ds3_client* client = get_client();
    ds3_request* request = ds3_init_verify_system_health_spectra_s3_request();
    ds3_health_verification_result_response* response = NULL;

    printf("-----Testing VerifySystemHealth-------\n");

    ds3_request_free(request);
    request = ds3_init_verify_system_health_spectra_s3_request();

    ds3_error* error = ds3_verify_system_health_spectra_s3_request(client, request, &response);
    BOOST_CHECK(error == NULL);
    BOOST_CHECK(response->ms_required_to_verify_data_planner_health >= 0);

    ds3_request_free(request);
    free_client(client);
    ds3_health_verification_result_response_free(response);
    BOOST_CHECK(error == NULL);
}

