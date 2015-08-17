#include <stdio.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( bulk_put ) {
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";
    uint64_t num_objs;

    printf("-----Testing Bulk PUT-------\n");

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);

    num_objs = response->num_objects;

    BOOST_CHECK(error == NULL);
    BOOST_CHECK_EQUAL(num_objs, 5);

    BOOST_CHECK(contains_object(response->objects, num_objs, "resources/beowulf.txt"));

    ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( prefix ) {
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";
    uint64_t num_objs;

    printf("-----Testing Prefix-------\n");

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    ds3_request_set_prefix(request, "resources/beo");
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);

    BOOST_CHECK(error == NULL);
    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 1);

    BOOST_CHECK(contains_object(response->objects, num_objs, "resources/beowulf.txt"));

    ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE( delimiter ) {
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";
    uint64_t num_objs;

    printf("-----Testing Delimiter-------\n");

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    ds3_request_set_delimiter(request, "/");
    error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);

    BOOST_CHECK(error == NULL);
    num_objs = response->num_objects;
    BOOST_CHECK_EQUAL(num_objs, 0);

    BOOST_CHECK_EQUAL(response->num_common_prefixes, 1);
    BOOST_CHECK_EQUAL(strcmp(response->common_prefixes[0]->value, "resources/"), 0);

    ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(marker) {
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "bucket_test_marker";
    uint64_t num_objs;

    printf("-----Testing Marker-------\n");

    populate_with_objects(client,bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    ds3_request_set_marker(request,"resources/sherlock_holmes.txt");
    error = ds3_get_bucket(client,request,&response);
    ds3_free_request(request);

    handle_error(error);
    num_objs = response->num_objects;

    BOOST_CHECK_EQUAL(num_objs, 3);
    BOOST_CHECK(contains_object(response->objects, num_objs, "resources/tale_of_two_cities.txt"));
    BOOST_CHECK(contains_object(response->objects, num_objs, "resources/ulysses.txt"));
    BOOST_CHECK(contains_object(response->objects, num_objs, "resources/ulysses_large.txt"));

    ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(max_keys) {
    ds3_request* request;
    ds3_error* error;
    ds3_get_bucket_response* response;
    ds3_client* client = get_client();
    const char* bucket_name = "bucket_test_max_keys";
    uint64_t num_objs;

    printf("-----Testing Max-Keys-------\n");

    populate_with_objects(client,bucket_name);

    request = ds3_init_get_bucket(bucket_name);
    ds3_request_set_max_keys(request,2);

    error = ds3_get_bucket(client,request,&response);
    ds3_free_request(request);

    handle_error(error);
    num_objs = response->num_objects;

    BOOST_CHECK_EQUAL(num_objs, 2);
    BOOST_CHECK(contains_object(response->objects, num_objs, "resources/beowulf.txt"));
    BOOST_CHECK(contains_object(response->objects, num_objs, "resources/sherlock_holmes.txt"));
    ds3_free_bucket_response(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(md5_checksum) {
    uint64_t i, n;
    const char* bucket_name = "bucket_test_md5";
    ds3_request* request = ds3_init_put_bucket(bucket_name);
    const char* books[] ={"resources/beowulf.txt"};
    ds3_client* client = get_client();
    ds3_error* error = ds3_put_bucket(client, request);
    ds3_bulk_object_list* obj_list;
    ds3_bulk_response* response;
    ds3_allocate_chunk_response* chunk_response;

    printf("-----Testing MD5 Checksum-------\n");

    ds3_free_request(request);
    handle_error(error);

    obj_list = ds3_convert_file_list(books, 1);
    request = ds3_init_put_bulk(bucket_name, obj_list);
    error = ds3_bulk(client, request, &response);

    ds3_free_request(request);
    handle_error(error);

    for (n = 0; n < response->list_size; n ++) {
      request = ds3_init_allocate_chunk(response->list[n]->chunk_id->value);
      error = ds3_allocate_chunk(client, request, &chunk_response);
      ds3_free_request(request);
      handle_error(error);
      BOOST_REQUIRE(chunk_response->retry_after == 0);
      BOOST_REQUIRE(chunk_response->objects != NULL);

      for (i = 0; i < chunk_response->objects->size; i++) {
          ds3_bulk_object bulk_object = chunk_response->objects->list[i];
          FILE* file = fopen(bulk_object.name->value, "r");

          request = ds3_init_put_object_for_job(bucket_name, bulk_object.name->value, bulk_object.offset,  bulk_object.length, response->job_id->value);
          ds3_request_set_md5(request,"rCu751L6xhB5zyL+soa3fg==");

          if (bulk_object.offset > 0) {
              fseek(file, bulk_object.offset, SEEK_SET);
          }
          error = ds3_put_object(client, request, file, ds3_read_from_file);
          ds3_free_request(request);
          fclose(file);
          handle_error(error);
      }
     ds3_free_allocate_chunk_response(chunk_response);
    }

    ds3_free_bulk_response(response);
    ds3_free_bulk_object_list(obj_list);
    clear_bucket(client, bucket_name);
    free_client(client);
}

