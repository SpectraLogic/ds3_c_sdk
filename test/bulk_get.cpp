#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>
#include "checksum.h"


#define FILE_TEMPLATE "temp-resources-"


struct checksum_result {
  char original_name[64];
  char tmp_name[64];
  int num_chunks;
  bool passed;
};

bool check_all_passed(uint32_t num_files, checksum_result* results){
    bool result = true;
    for(uint32_t i=0; i<num_files; i++){
        result=result && results[i].passed;
    }
    return result;
}

uint32_t get_number_of_chunks(uint32_t num_files, checksum_result* results, const char* filename){
    for(uint32_t i=0; i<num_files; i++){
      if(g_strcmp0(results[i].original_name, filename)==0){
        return results[i].num_chunks;
      }
    }
    return 0;
}

uint32_t get_sum_of_chunks(uint32_t num_files, checksum_result* results){
    uint32_t result = 0;
    for(uint32_t i=0; i<num_files; i++){
        result += results[i].num_chunks;
    }
    return result;
}

void checkChunkResponse(ds3_client* client, uint32_t num_files, ds3_get_available_chunks_response* chunk_response, checksum_result* results){
    uint64_t i, n, k;
    uint64_t max_file_index = 0;
    int64_t file_index = 0;
    ds3_request* request = NULL;
    ds3_error* error = NULL;

    for (i = 0; i < chunk_response->object_list->list_size; i++) {
        ds3_bulk_object_list* chunk_object_list = chunk_response->object_list->list[i];
        for(n = 0; n < chunk_object_list->size; n++) {
            FILE* w_file;
            ds3_bulk_object current_obj = chunk_object_list->list[n];
	    file_index = -1;
	    for(k = 0; k < max_file_index; k++){
	      if(g_strcmp0(current_obj.name->value, results[k].original_name) == 0){
		file_index = k;
		break;
	      }
	    }
	    if(file_index == -1){
                file_index=max_file_index;
                max_file_index++;
		
		memcpy(results[file_index].tmp_name+5, current_obj.name->value, 64-5);
		memcpy(results[file_index].tmp_name, FILE_TEMPLATE, 15);
		
		memcpy(results[file_index].original_name, current_obj.name->value, 64);
	    }
	    results[file_index].num_chunks++;
	    
            request = ds3_init_get_object_for_job(chunk_response->object_list->bucket_name->value, current_obj.name->value, current_obj.offset, chunk_response->object_list->job_id->value);
	    
            w_file = fopen(results[file_index].tmp_name, "a+");
	    fseek(w_file, current_obj.offset, SEEK_SET);
	    
            error = ds3_get_object(client, request, w_file, ds3_write_to_file);
            ds3_free_request(request);
            fclose(w_file);
            handle_error(error);
        }
    }

    for (i = 0; i < max_file_index; i++) {
            printf("------Performing Data Integrity Test-------\n");
	    results[i].passed=compare_hash(results[i].original_name, results[i].tmp_name);
	    unlink(results[i].tmp_name);
    }
}

ds3_get_available_chunks_response* ensure_available_chunks(ds3_client* client, ds3_str* job_id){
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    bool retry_get;
    ds3_get_available_chunks_response* chunk_response;
    do {
        retry_get = false;
	request = ds3_init_get_available_chunks(job_id->value);

	error = ds3_get_available_chunks(client, request, &chunk_response);

	ds3_free_request(request);
	  
	BOOST_REQUIRE(error == NULL);
	
	BOOST_REQUIRE(chunk_response != NULL);

	if (chunk_response->object_list->list_size == 0) {
	    // if this happens we need to try the request
	    retry_get = true;
	    BOOST_TEST_MESSAGE( "Hit retry, sleeping for: " << chunk_response->retry_after) ;
	    sleep(chunk_response->retry_after);
	}
    } while(retry_get);
    return chunk_response;
}

BOOST_AUTO_TEST_CASE( bulk_get ) {
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_bulk_response* completed_job = NULL;
    ds3_bulk_response* bulk_response = NULL;
    ds3_bulk_object_list* object_list = NULL;
    ds3_get_available_chunks_response* chunk_response = NULL;

    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";

    const uint32_t num_files=5;
    object_list=default_object_list();
    
    request=populate_bulk_return_request(client, bucket_name, object_list);
    bulk_response=populate_bulk_return_response(client, request);
    populate_with_objects_from_bulk(client, bucket_name, bulk_response);

    request = ds3_init_get_bulk(bucket_name, object_list, NONE);
    error = ds3_bulk(client, request, &bulk_response);

    ds3_free_request(request);
    ds3_free_bulk_object_list(object_list);

    BOOST_REQUIRE(error == NULL);

    chunk_response = ensure_available_chunks(client, bulk_response->job_id);

    BOOST_REQUIRE(error == NULL);

    checksum_result* checksum_results=(checksum_result*) calloc(num_files, sizeof(checksum_result));
    checkChunkResponse(client, num_files, chunk_response, checksum_results);
    
    BOOST_CHECK(check_all_passed(num_files, checksum_results) == true);
    BOOST_CHECK(get_sum_of_chunks(num_files, checksum_results) == num_files);
    
    free(checksum_results);

    // check to make sure that the 'job' has completed
    request = ds3_init_get_job(bulk_response->job_id->value);
    error = ds3_get_job(client, request, &completed_job);

    BOOST_CHECK(completed_job != NULL);
    BOOST_CHECK(completed_job->status == COMPLETED);

    ds3_free_request(request);
    ds3_free_available_chunks_response(chunk_response);
    ds3_free_bulk_response(completed_job);
    ds3_free_bulk_response(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
    handle_error(error);
}

BOOST_AUTO_TEST_CASE( max_upload_size ) {
    // add changes to other bulk_gets
    ds3_request* request = NULL;
    ds3_error* error = NULL;
    ds3_bulk_response* completed_job = NULL;
    ds3_bulk_response* bulk_response = NULL;
    ds3_bulk_object_list* object_list = NULL;
    ds3_get_available_chunks_response* chunk_response = NULL;

    ds3_client* client = get_client();
    const char* bucket_name = "unit_test_bucket";

    const uint32_t num_files=6;
    const char* books[num_files] = {"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/tale_of_two_cities.txt", "resources/ulysses.txt", "resources/ulysses_large.txt", "resources/ulysses_118mb.txt"};
    object_list=ds3_convert_file_list(books, num_files);
    
    request=populate_bulk_return_request(client, bucket_name, object_list);
    ds3_request_set_max_upload_size(request, 10485760);
    bulk_response=populate_bulk_return_response(client, request);
    populate_with_objects_from_bulk(client, bucket_name, bulk_response);

    request = ds3_init_get_bulk(bucket_name, object_list, NONE);
    error = ds3_bulk(client, request, &bulk_response);

    ds3_free_request(request);
    ds3_free_bulk_object_list(object_list);

    BOOST_REQUIRE(error == NULL);

    chunk_response = ensure_available_chunks(client, bulk_response->job_id);

    BOOST_REQUIRE(error == NULL);

    checksum_result* checksum_results=(checksum_result*) calloc(num_files, sizeof(checksum_result));
    checkChunkResponse(client, num_files, chunk_response, checksum_results);
    
    BOOST_CHECK(check_all_passed(num_files, checksum_results) == true);
    BOOST_CHECK(get_number_of_chunks(num_files, checksum_results, "resources/ulysses_large.txt") == 2);
    BOOST_CHECK(get_number_of_chunks(num_files, checksum_results, "resources/ulysses_118mb.txt") == 12);
    BOOST_CHECK(get_sum_of_chunks(num_files, checksum_results) == 18);
    
    free(checksum_results);

    // check to make sure that the 'job' has completed
    request = ds3_init_get_job(bulk_response->job_id->value);
    error = ds3_get_job(client, request, &completed_job);

    BOOST_CHECK(completed_job != NULL);
    BOOST_CHECK(completed_job->status == COMPLETED);

    ds3_free_request(request);
    ds3_free_available_chunks_response(chunk_response);
    ds3_free_bulk_response(completed_job);
    ds3_free_bulk_response(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
    handle_error(error);
}

BOOST_AUTO_TEST_CASE( convert_list_helper ) {
    const char* books[4] ={"beowulf.txt", "sherlock_holmes.txt", "tale_of_two_cities.txt", "ulysses.txt"};
    ds3_bulk_object_list* obj_list;

    obj_list = ds3_convert_file_list_with_basepath(books, 4, "resources/");

    BOOST_CHECK(strcmp(obj_list->list[0].name->value, "beowulf.txt") == 0);
    BOOST_CHECK(obj_list->list[0].length == 294059);

    ds3_free_bulk_object_list(obj_list);
}

BOOST_AUTO_TEST_CASE( directory_size ) {
    const char* books[1] ={"resources"};
    ds3_bulk_object_list* obj_list;

    obj_list = ds3_convert_file_list_with_basepath(books, 1, NULL);

    BOOST_CHECK(strcmp(obj_list->list[0].name->value, "resources") == 0);
    BOOST_CHECK(obj_list->list[0].length == 0);

    ds3_free_bulk_object_list(obj_list);
}
