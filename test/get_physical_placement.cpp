#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( get_physical_placment ){

    printf("-----Testing GetPhysicalPlacement -------\n");

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
    ds3_get_physical_placement_response* get_physical_placement_response = NULL;
    uint64_t num_tapes;

    ds3_client* client = get_client();
    const char* bucket_name = "get_physical_placement_test_bucket";
    populate_with_objects(client, bucket_name);

    ds3_bulk_object_list* object_list = ds3_init_bulk_object_list(1);

    ds3_bulk_object obj;
    memset(&obj, 0, sizeof(ds3_bulk_object));
    obj.name = ds3_str_init("resources/not-beowulf.txt");
    object_list->list[0] = obj;

    request = ds3_init_get_physical_placement(bucket_name, object_list);
    error = ds3_get_physical_placement(client, request, &get_physical_placement_response);

    ds3_free_request(request);
    ds3_free_bulk_object_list(object_list);
    handle_error(error);
    if (get_physical_placement_response == NULL) {
        BOOST_MESSAGE("The response was null without an error, which means we have no tape backend.  Skipping the test.");
        clear_bucket(client, bucket_name);
        ds3_free_client(client);
        return;
    }

    num_tapes = get_physical_placement_response->num_tapes;
    BOOST_CHECK_EQUAL(num_tapes, 0);

    ds3_free_get_physical_placement_response(get_physical_placement_response);
    clear_bucket(client, bucket_name);
    free_client(client);
}

// For this test to actually work, we need an object to be already written to tape.
/*
BOOST_AUTO_TEST_CASE( get_physical_placment_full_details ){
    printf("-----Testing GetPhysicalPlacement with full_details -------\n");

    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_get_physical_placement_response* get_physical_placement_response = NULL;
    uint64_t num_tapes;

    ds3_client* client = get_client();
    const char* bucket_name = "Spectra-BlackPearl-Backup";

    ds3_bulk_object_list* object_list = ds3_init_bulk_object_list(1);

    ds3_bulk_object obj;
    memset(&obj, 0, sizeof(ds3_bulk_object));
    obj.name = ds3_str_init("full_backup_2015-08-31_03-00-00.tar.gz");
    object_list->list[0] = obj;

    request = ds3_init_get_physical_placement(bucket_name, object_list);
    error = ds3_get_physical_placement(client, request, &get_physical_placement_response);

    ds3_free_request(request);
    ds3_free_bulk_object_list(object_list);
    handle_error(error);
    if (get_physical_placement_response == NULL) {
        BOOST_MESSAGE("The response was null without an error, which means we have no tape backend.  Skipping the test.");
        clear_bucket(client, bucket_name);
        ds3_free_client(client);
        return;
    }

    num_tapes = get_physical_placement_response->num_tapes;
    BOOST_CHECK(num_tapes > 0);

    BOOST_CHECK_EQUAL(get_physical_placement_response->tapes[0].assigned_to_bucket, True);
    BOOST_CHECK(get_physical_placement_response->tapes[0].available_raw_capacity > 0);
    BOOST_CHECK(get_physical_placement_response->tapes[0].barcode != NULL);
    BOOST_CHECK(get_physical_placement_response->tapes[0].bucket_id != NULL);
    BOOST_CHECK(get_physical_placement_response->tapes[0].id != NULL);
    BOOST_CHECK(get_physical_placement_response->tapes[0].last_accessed != NULL);
    BOOST_CHECK(get_physical_placement_response->tapes[0].last_checkpoint != NULL);
    BOOST_CHECK(get_physical_placement_response->tapes[0].last_modified != NULL);
    BOOST_CHECK(get_physical_placement_response->tapes[0].partition_id != NULL);
    BOOST_CHECK(get_physical_placement_response->tapes[0].serial_number != NULL);
    BOOST_CHECK(get_physical_placement_response->tapes[0].total_raw_capacity > 0);

    ds3_free_get_physical_placement_response(get_physical_placement_response);
    free_client(client);
}
*/
