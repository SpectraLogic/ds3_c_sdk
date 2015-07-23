#include <stdbool.h>
#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( get_objects ) {
    ds3_client* client = get_client();
    ds3_get_objects_response* response;
    const char* bucket_name = "search_bucket_test";
    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_objects(bucket_name);
    _set_query_param(request, "bucket_id", bucket_name);
    ds3_request_set_name(request, "%txt%");

    ds3_error* error = ds3_get_objects(client, request, &response);

    handle_error(error);

    ds3_free_request(request);
    ds3_free_service_response(response);
    free_client(client);
}