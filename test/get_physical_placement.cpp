#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( get_physical_placment ){
    /* For this test to actually work, we would need to wait until the object
     * actually gets put onto tape (which could take minutes).
     * The response for an object that doesn't exist is the same as that
     * for an object that exists but is not on tape yet.
     * Just ask for a non-existent oject to test that an empty response is
     * successfully generated.
     * Use populate_with_objects to ensure the bucket gets created.
    */
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_get_bucket_response* response = NULL;
    ds3_bulk_response* bulk_response = NULL;
    ds3_get_physical_placement_response* get_physical_placement_response = NULL;
    uint64_t num_tapes;

    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);
    BOOST_REQUIRE(error == NULL);

    ds3_bulk_object_list* object_list = ds3_init_bulk_object_list(1);

    ds3_bulk_object obj;
    memset(&obj, 0, sizeof(ds3_bulk_object));
    obj.name = ds3_str_init("resources/not-beowulf.txt");
    object_list->list[0] = obj;

    request = ds3_init_get_physical_placement(bucket_name, object_list);
    error = ds3_get_physical_placement(client, request, &get_physical_placement_response);

    ds3_free_request(request);
    ds3_free_bulk_object_list(object_list);
    num_tapes = get_physical_placement_response->num_tapes;
    BOOST_CHECK_EQUAL(num_tapes, 0);

    BOOST_REQUIRE(error == NULL);

    ds3_free_bulk_response(bulk_response);
    ds3_free_get_phsyical_placement_response(get_physical_placement_response);
    ds3_free_bucket_response(response);
    clear_bucket(client, bucket_name);
}
