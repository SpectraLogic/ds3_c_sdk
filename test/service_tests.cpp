#include <stdbool.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( get_service ) {
    ds3_client* client = get_client();
    ds3_request* request = ds3_init_get_service();
    ds3_get_service_response* response;

    ds3_error* error = ds3_get_service(client, request, &response);

    BOOST_CHECK(error == NULL);

    ds3_free_request(request);
    ds3_free_service_response(response);
}

BOOST_AUTO_TEST_CASE( put_bucket) {
    ds3_client* client = get_client();
    uint64_t i;
    bool found = false;
    const char* bucket_name = "unit_test_bucket";
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    ds3_get_service_response* response;

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

    BOOST_CHECK(error == NULL);
}
