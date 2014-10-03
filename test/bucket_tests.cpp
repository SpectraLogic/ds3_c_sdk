#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

static void populate_with_objects(const ds3_client* client, const char* bucket_name) {
    uint64_t i;
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    const char* books[4] ={"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/tale_of_two_cities.txt", "resources/ulysses.txt"};
    ds3_error* error = ds3_put_bucket(client, request);
    ds3_bulk_object_list* obj_list; 
    ds3_bulk_response* response;
    ds3_free_request(request);
   
    BOOST_CHECK(error == NULL);
    
    obj_list = ds3_convert_file_list(books, 4);
    request = ds3_init_put_bulk(bucket_name, obj_list);
    error = ds3_bulk(client, request, &response);

    BOOST_CHECK(error == NULL);

    ds3_free_request(request);
    for (i = 0; i < 4; i++) {
        ds3_bulk_object bulk_object = obj_list->list[i];
        FILE* file = fopen(bulk_object.name->value, "r");
        
        request = ds3_init_put_object(bucket_name, bulk_object.name->value, bulk_object.length);
        error = ds3_put_object(client, request, file, ds3_read_from_file);
        ds3_free_request(request);
        
        BOOST_CHECK(error == NULL);
    }   

    ds3_free_bulk_response(response);
    ds3_free_bulk_object_list(obj_list);
}

BOOST_AUTO_TEST_CASE( bulk_put ) {
    ds3_client* client = get_client();
    const char* bucket_name = "test_bucket";
    
    populate_with_objects(client, bucket_name);
    clear_bucket(client, bucket_name);
}
