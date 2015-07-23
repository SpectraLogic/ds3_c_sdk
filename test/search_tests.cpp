#include <stdbool.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( get_objects ) {
    uint64_t num_objs;
    ds3_client* client = get_client();
    ds3_get_objects_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects(bucket_name);
    ds3_request_set_name(request, "%txt%");

    ds3_error* error = ds3_get_objects(client, request, &response);

    handle_error(error);
    num_objs = response->num_objects;
	
    BOOST_CHECK_EQUAL(num_objs, 5);

    ds3_free_request(request);
    ds3_free_objects_response(response);
    clear_bucket(client, bucket_name);
    free_client(client);
}