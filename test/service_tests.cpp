#include <stdbool.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( get_service ) {
    ds3_client* client = get_client();
    ds3_request* request = ds3_init_get_service();
    ds3_get_service_response* response;

    printf("-----Testing GET service-------\n");

    ds3_error* error = ds3_get_service(client, request, &response);

    BOOST_CHECK(error == NULL);

    ds3_free_request(request);
    ds3_free_service_response(response);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( put_bucket) {
    ds3_client* client = get_client();
    uint64_t i;
    bool found = false;
    const char* bucket_name = "unit_test_bucket";
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    ds3_get_service_response* response;

    printf("-----Testing GET service after PUT bucket-------\n");

    ds3_error* error = ds3_put_bucket(client, request);

    BOOST_CHECK(error == NULL);

    ds3_free_request(request);
    request = ds3_init_get_service();

    error = ds3_get_service(client, request, &response);

    BOOST_CHECK(error == NULL);

    for (i = 0; i < response->num_buckets; i++) {
        fprintf(stderr, "Expected Name (%s) actual (%s)\n", bucket_name, response->buckets[i].name->value);
        if (strcmp(bucket_name, response->buckets[i].name->value) == 0) {
            found = true;
            break;
        }
    }

    ds3_free_request(request);
    ds3_free_service_response(response);

    BOOST_CHECK(found);

    request = ds3_init_delete_bucket(bucket_name);
    error = ds3_delete_bucket(client, request);
    ds3_free_request(request);

    free_client(client);
    BOOST_CHECK(error == NULL);
}

BOOST_AUTO_TEST_CASE( get_system_information ) {
    ds3_client* client = get_client();
    ds3_request* request = ds3_init_get_system_information();
    ds3_get_system_information_response* response;

    printf("-----Testing GET system_information-------\n");

    ds3_error* error = ds3_get_system_information(client, request, &response);
    BOOST_CHECK(error == NULL);

    BOOST_CHECK(response->api_version != NULL);
    BOOST_CHECK(response->serial_number != NULL);

    BOOST_CHECK(response->build_information != NULL);
    BOOST_CHECK(response->build_information->branch != NULL);
    BOOST_CHECK(response->build_information->revision != NULL);
    BOOST_CHECK(response->build_information->version != NULL);

    ds3_free_request(request);
    ds3_free_get_system_information(response);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( verify_system_health ) {
    ds3_client* client = get_client();
    uint64_t response_time_ms = -42;
    ds3_request* request = ds3_init_verify_system_health();

    printf("-----Testing VerifySystemHealth-------\n");

    ds3_free_request(request);
    request = ds3_init_verify_system_health();

    ds3_error* error = ds3_verify_system_health(client, request, &response_time_ms);
    BOOST_CHECK(error == NULL);
    BOOST_CHECK(response_time_ms >= 0);

    ds3_free_request(request);
    free_client(client);
    BOOST_CHECK(error == NULL);
}
