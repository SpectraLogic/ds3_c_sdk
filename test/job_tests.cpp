/*
 * ******************************************************************************
 *   Copyright 2014-2016 Spectra Logic Corporation. All Rights Reserved.
 *   Licensed under the Apache License, Version 2.0 (the "License"). You may not use
 *   this file except in compliance with the License. A copy of the License is located at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   or in the "license" file accompanying this file.
 *   This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 *   CONDITIONS OF ANY KIND, either express or implied. See the License for the
 *   specific language governing permissions and limitations under the License.
 * ****************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "ds3.h"
#include "ds3_utils.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(get_job){
    ds3_request* request;
    ds3_error* error;
    ds3_list_bucket_result_response* response = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_master_object_list_response* response_get = NULL;
    ds3_client* client = get_client();
    ds3_bulk_object_list_response* object_list = NULL;
    const char* bucket_name = "bucket_test_get_job";

    printf("-----Testing Get Job-------\n");

    populate_with_objects(client, bucket_name);

    request = ds3_init_get_bucket_request(bucket_name);
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    object_list = ds3_convert_object_list((const ds3_contents_response**)response->objects, response->num_objects);
    ds3_list_bucket_result_response_free(response);

    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, object_list);
    ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(request, DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_free(request);
    ds3_bulk_object_list_response_free(object_list);

    request = ds3_init_get_job_spectra_s3_request(bulk_response->job_id->value);
    error = ds3_get_job_spectra_s3_request(client, request, &response_get);
    ds3_master_object_list_response_free(bulk_response);
    handle_error(error);

    BOOST_CHECK(response_get != NULL);
    BOOST_CHECK(response_get->status == DS3_JOB_STATUS_IN_PROGRESS);
    ds3_request_free(request);
    ds3_master_object_list_response_free(response_get);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(cancel_job){
    ds3_request* request;
    ds3_error* error;
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_client* client = get_client();

    const char* bucket_name = "bucket_test_cancel_job";

    printf("-----Testing Cancel Job-------\n");

    ds3_str* job_id = populate_with_empty_objects(client, bucket_name);

    request = ds3_init_cancel_job_spectra_s3_request(job_id->value);
    ds3_request_set_force(request, True);
    error = ds3_cancel_job_spectra_s3_request(client, request);
    handle_error(error);
    ds3_request_free(request);

    request = ds3_init_get_job_spectra_s3_request(job_id->value);
    error = ds3_get_job_spectra_s3_request(client, request, &bulk_response);
    ds3_str_free(job_id);
    ds3_request_free(request);
    handle_error(error);

    BOOST_CHECK(bulk_response->status == DS3_JOB_STATUS_CANCELED);
    ds3_master_object_list_response_free(bulk_response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(create_aggregate_job){
    ds3_client* client = get_client();

    const char* bucket_name = "bucket_test_create_aggregate_job";

    printf("-----Testing Create Aggregate Job-------\n");

    ds3_bulk_object_list_response* obj_list = default_object_list();
    ds3_request* request = populate_bulk_return_request(client, bucket_name, obj_list);
    ds3_request_set_aggregating(request, True);
    ds3_master_object_list_response* response = populate_bulk_return_response(client, request);

    BOOST_CHECK(response->aggregating == True);
    ds3_master_object_list_response_free(response);
    ds3_bulk_object_list_response_free(obj_list);
    clear_bucket(client, bucket_name);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(get_jobs){
    ds3_client* client = get_client();
    ds3_error* error = NULL;
    ds3_request* request = NULL;

    ds3_list_bucket_result_response* get_bucket_response = NULL;
    ds3_master_object_list_response* bulk_response = NULL;
    ds3_job_list_response* get_jobs_response = NULL;

    const char* bucket1_name = "bucket_test_get_job1";
    ds3_bulk_object_list_response* bucket1_object_list = NULL;
    ds3_str* bucket1_job_id = NULL;
    ds3_bool found_bucket1_job = False;
    const char* bucket2_name = "bucket_test_get_job2";
    ds3_bulk_object_list_response* bucket2_object_list = NULL;
    ds3_str* bucket2_job_id = NULL;
    ds3_bool found_bucket2_job = False;

    printf("-----Testing Get Jobs-------\n");

    /* create bucket1 with objects */
    populate_with_objects(client, bucket1_name);

    request = ds3_init_get_bucket_request(bucket1_name);
    error = ds3_get_bucket_request(client, request, &get_bucket_response);
    ds3_request_free(request);
    handle_error(error);

    // retain object_list for ds3_init_get_bulk
    bucket1_object_list = ds3_convert_object_list((const ds3_contents_response**)get_bucket_response->objects, get_bucket_response->num_objects);
    ds3_list_bucket_result_response_free(get_bucket_response);

    /* create bucket2 with objects */
    request = ds3_init_get_bucket_request(bucket2_name);
    populate_with_objects(client, bucket2_name);
    error = ds3_get_bucket_request(client, request, &get_bucket_response);
    ds3_request_free(request);
    handle_error(error);

    // retain object_list for ds3_init_get_bulk
    bucket2_object_list = ds3_convert_object_list((const ds3_contents_response**)get_bucket_response->objects, get_bucket_response->num_objects);
    ds3_list_bucket_result_response_free(get_bucket_response);

    /* init bulk get bucket1 */
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket1_name, bucket1_object_list);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(request, DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE);
    bucket1_job_id = ds3_str_dup(bulk_response->job_id);
    ds3_bulk_object_list_response_free(bucket1_object_list);
    ds3_master_object_list_response_free(bulk_response);
    ds3_request_free(request);
    handle_error(error);

    /* init bulk get bucket2 */
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket2_name, bucket2_object_list);
    ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(request, DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    bucket2_job_id = ds3_str_dup(bulk_response->job_id);
    ds3_bulk_object_list_response_free(bucket2_object_list);
    ds3_master_object_list_response_free(bulk_response);
    ds3_request_free(request);
    handle_error(error);

    /* GET jobs */
    request = ds3_init_get_jobs_spectra_s3_request();
    error = ds3_get_jobs_spectra_s3_request(client, request, &get_jobs_response);
    ds3_request_free(request);
    handle_error(error);

    /* Assert valid response contents */
    BOOST_CHECK(get_jobs_response != NULL);
    uint8_t job_index;
    for( job_index = 0; job_index < get_jobs_response->num_jobs; job_index++ ) {
        ds3_job_response* job_response = get_jobs_response->jobs[job_index];
        BOOST_CHECK(job_response->bucket_name->value != NULL);
        BOOST_CHECK(job_response->job_id->value != NULL);
        BOOST_CHECK(job_response->start_date->value != NULL);
        BOOST_CHECK(job_response->user_id->value != NULL);
        BOOST_CHECK(job_response->user_name->value != NULL);
        if(0 == strcmp(job_response->job_id->value, bucket1_job_id->value)) {
            found_bucket1_job = True;
        } else if(0 == strcmp(job_response->job_id->value, bucket2_job_id->value)) {
            found_bucket2_job = True;
        }
    }

    /* teardown */
    clear_bucket(client, bucket1_name);
    clear_bucket(client, bucket2_name);
    free_client(client);
    ds3_str_free(bucket1_job_id);
    ds3_str_free(bucket2_job_id);
    ds3_job_list_response_free(get_jobs_response);
    BOOST_CHECK(True == found_bucket1_job);
    BOOST_CHECK(True == found_bucket2_job);
}

BOOST_AUTO_TEST_CASE( GetJobToReplicateRequestHandler_response_type_not_parsed ) {
    printf("-----Testing GetJobToReplicateRequestHandler response type parsing-------\n");

    const char* bucket_name = "bucket_test_response_type_parsing";
    ds3_client* client = get_client();
    ds3_request* request;
    ds3_error* error;

    ds3_list_bucket_result_response* response = NULL;

    ds3_master_object_list_response* bulk_response = NULL;
    ds3_bulk_object_list_response* object_list = NULL;

    ds3_str* response_str = NULL;
    ds3_str* job_id = NULL;

    populate_with_objects(client, bucket_name);

    // Get bucket contents
    request = ds3_init_get_bucket_request(bucket_name);
    error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    object_list = ds3_convert_object_list((const ds3_contents_response**)response->objects, response->num_objects);
    ds3_list_bucket_result_response_free(response);

    // Create a bulk_get job
    request = ds3_init_get_bulk_job_spectra_s3_request(bucket_name, object_list);
    ds3_request_set_chunk_client_processing_order_guarantee_ds3_job_chunk_client_processing_order_guarantee(request, DS3_JOB_CHUNK_CLIENT_PROCESSING_ORDER_GUARANTEE_NONE);
    error = ds3_get_bulk_job_spectra_s3_request(client, request, &bulk_response);
    job_id = ds3_str_dup(bulk_response->job_id);
    ds3_request_free(request);
    ds3_error_free(error);
    ds3_bulk_object_list_response_free(object_list);

    // Replicate the job
    request = ds3_init_get_job_to_replicate_spectra_s3_request(job_id->value);
    error = ds3_get_job_to_replicate_spectra_s3_request(client, request, &response_str);

    BOOST_CHECK(error == NULL);
    BOOST_CHECK(response_str->size > 0);
    ds3_str_free(response_str);
    ds3_str_free(job_id);
    ds3_error_free(error);
    ds3_request_free(request);

    ds3_master_object_list_response_free(bulk_response);
    clear_bucket(client, bucket_name);
    free_client(client);
}

static ds3_error* mock_net_callback_active_jobs_data(const ds3_client* client,
                                                    const ds3_request* _request,
                                                    void* read_user_struct,
                                                    size_t (*read_handler_func)(void*, size_t, size_t, void*),
                                                    void* write_user_struct,
                                                    size_t (*write_handler_func)(void*, size_t, size_t, void*),
                                                    ds3_string_multimap** return_headers) {
    const char* response_body = "<Data><Job><Aggregating>false</Aggregating><BucketId>47e03263-ca76-4c3c-90ff-d58a251fd826</BucketId><CachedSizeInBytes>0</CachedSizeInBytes><ChunkClientProcessingOrderGuarantee>IN_ORDER</ChunkClientProcessingOrderGuarantee><CompletedSizeInBytes>0</CompletedSizeInBytes><CreatedAt>2026-04-30T17:01:36.331Z</CreatedAt><DeadJobCleanupAllowed>true</DeadJobCleanupAllowed><ErrorMessage/><Id>5cc5235b-edaf-46b8-ac4d-d45157141668</Id><ImplicitJobIdResolution>false</ImplicitJobIdResolution><MinimizeSpanningAcrossMedia>false</MinimizeSpanningAcrossMedia><Naked>false</Naked><Name>Untitled</Name><OriginalSizeInBytes>0</OriginalSizeInBytes><Priority>URGENT</Priority><Protected>false</Protected><Rechunked/><Replicating>false</Replicating><RequestType>PUT</RequestType><Restore>NO</Restore><Truncated>false</Truncated><TruncatedDueToTimeout>false</TruncatedDueToTimeout><UserId>d142810b-c3d4-45dc-be4b-e86b85029ae7</UserId><VerifyAfterWrite>false</VerifyAfterWrite></Job></Data>";
    size_t body_len = strlen(response_body);
    read_handler_func((void*)response_body, 1, body_len, read_user_struct);
    return NULL;
}

static ds3_error* mock_net_callback_active_jobs_jobs(const ds3_client* client,
                                                    const ds3_request* _request,
                                                    void* read_user_struct,
                                                    size_t (*read_handler_func)(void*, size_t, size_t, void*),
                                                    void* write_user_struct,
                                                    size_t (*write_handler_func)(void*, size_t, size_t, void*),
                                                    ds3_string_multimap** return_headers) {
    const char* response_body = "<Jobs><Job><Aggregating>false</Aggregating><BucketId>47e03263-ca76-4c3c-90ff-d58a251fd826</BucketId><CachedSizeInBytes>0</CachedSizeInBytes><ChunkClientProcessingOrderGuarantee>IN_ORDER</ChunkClientProcessingOrderGuarantee><CompletedSizeInBytes>0</CompletedSizeInBytes><CreatedAt>2026-04-30T17:01:36.331Z</CreatedAt><DeadJobCleanupAllowed>true</DeadJobCleanupAllowed><ErrorMessage/><Id>5cc5235b-edaf-46b8-ac4d-d45157141668</Id><ImplicitJobIdResolution>false</ImplicitJobIdResolution><MinimizeSpanningAcrossMedia>false</MinimizeSpanningAcrossMedia><Naked>false</Naked><Name>Untitled</Name><OriginalSizeInBytes>0</OriginalSizeInBytes><Priority>URGENT</Priority><Protected>false</Protected><Rechunked/><Replicating>false</Replicating><RequestType>PUT</RequestType><Restore>NO</Restore><Truncated>false</Truncated><TruncatedDueToTimeout>false</TruncatedDueToTimeout><UserId>d142810b-c3d4-45dc-be4b-e86b85029ae7</UserId><VerifyAfterWrite>false</VerifyAfterWrite></Job></Jobs>";
    size_t body_len = strlen(response_body);
    read_handler_func((void*)response_body, 1, body_len, read_user_struct);
    return NULL;
}

static ds3_error* mock_net_callback_canceled_jobs_data(const ds3_client* client,
                                                      const ds3_request* _request,
                                                      void* read_user_struct,
                                                      size_t (*read_handler_func)(void*, size_t, size_t, void*),
                                                      void* write_user_struct,
                                                      size_t (*write_handler_func)(void*, size_t, size_t, void*),
                                                      ds3_string_multimap** return_headers) {
    const char* response_body = "<Data><CanceledJob><BucketId>f7de8012-cd75-41f1-8117-3748714c3ae1</BucketId><CachedSizeInBytes>0</CachedSizeInBytes><CanceledDueToTimeout>false</CanceledDueToTimeout><ChunkClientProcessingOrderGuarantee>IN_ORDER</ChunkClientProcessingOrderGuarantee><CompletedSizeInBytes>0</CompletedSizeInBytes><CreatedAt>2026-04-30T17:01:36.450Z</CreatedAt><DateCanceled>2026-04-30T17:01:36.450Z</DateCanceled><ErrorMessage/><Id>be40114a-0833-4b12-a4d1-33ea379dae31</Id><Naked>false</Naked><Name>Untitled</Name><OriginalSizeInBytes>0</OriginalSizeInBytes><Priority>URGENT</Priority><Rechunked/><RequestType>PUT</RequestType><Truncated>false</Truncated><UserId>f2e3bcaf-d2f8-4944-9444-4b826ac75d94</UserId></CanceledJob></Data>";
    size_t body_len = strlen(response_body);
    read_handler_func((void*)response_body, 1, body_len, read_user_struct);
    return NULL;
}

static ds3_error* mock_net_callback_completed_jobs_data(const ds3_client* client,
                                                       const ds3_request* _request,
                                                       void* read_user_struct,
                                                       size_t (*read_handler_func)(void*, size_t, size_t, void*),
                                                       void* write_user_struct,
                                                       size_t (*write_handler_func)(void*, size_t, size_t, void*),
                                                       ds3_string_multimap** return_headers) {
    const char* response_body = "<Data><CompletedJob><BucketId>598e313c-20b1-4eec-aea9-72b5d99da5a8</BucketId><CachedSizeInBytes>0</CachedSizeInBytes><ChunkClientProcessingOrderGuarantee>IN_ORDER</ChunkClientProcessingOrderGuarantee><CompletedSizeInBytes>0</CompletedSizeInBytes><CreatedAt>2026-04-30T17:01:36.824Z</CreatedAt><DateCompleted>2026-04-30T17:01:36.832Z</DateCompleted><ErrorMessage/><Id>e3fe3d7f-2b63-4505-bcc5-38a61eebfdf7</Id><Naked>false</Naked><Name>Untitled</Name><OriginalSizeInBytes>0</OriginalSizeInBytes><Priority>URGENT</Priority><Rechunked/><RequestType>PUT</RequestType><Truncated>false</Truncated><UserId>c31a162a-aed9-4a3d-83cc-fd41cd494622</UserId></CompletedJob></Data>";
    size_t body_len = strlen(response_body);
    read_handler_func((void*)response_body, 1, body_len, read_user_struct);
    return NULL;
}

static ds3_error* mock_net_callback_get_jobs_jobs(const ds3_client* client,
                                                 const ds3_request* _request,
                                                 void* read_user_struct,
                                                 size_t (*read_handler_func)(void*, size_t, size_t, void*),
                                                 void* write_user_struct,
                                                 size_t (*write_handler_func)(void*, size_t, size_t, void*),
                                                 ds3_string_multimap** return_headers) {
    const char* response_body = "<Jobs><Job Aggregating=\"false\" BucketName=\"default-bucket-name\" CachedSizeInBytes=\"0\" ChunkClientProcessingOrderGuarantee=\"IN_ORDER\" CompletedSizeInBytes=\"0\" JobId=\"398181ff-fea4-456b-8f3f-50fb8e6c0d87\" Naked=\"false\" Name=\"Untitled\" OriginalSizeInBytes=\"0\" Priority=\"URGENT\" RequestType=\"PUT\" StartDate=\"2025-02-10T16:15:05.856Z\" Status=\"IN_PROGRESS\" UserId=\"58551a89-e2cc-414b-9e14-c92142820e2e\" UserName=\"user1\"><Nodes/></Job></Jobs>";
    size_t body_len = strlen(response_body);
    read_handler_func((void*)response_body, 1, body_len, read_user_struct);
    return NULL;
}

BOOST_AUTO_TEST_CASE(get_active_jobs_xml_data_root) {
    char* endpoint = getenv("DS3_ENDPOINT");
    bool is_live = (endpoint != NULL);
    ds3_client* client = NULL;

    printf("Test get_active_jobs_xml_data_root: is_live=%s, endpoint=%s\n", is_live ? "true" : "false", endpoint ? endpoint : "NULL");

    if (!is_live) {
        client = get_client();
        ds3_client_register_net(client, mock_net_callback_active_jobs_data);
    } else {
        ds3_error* env_error = ds3_create_client_from_env(&client);
        if (env_error != NULL) {
            handle_error(env_error);
            return;
        }
    }

    ds3_request* request = ds3_init_get_active_jobs_spectra_s3_request();
    ds3_active_job_list_response* response = NULL;
    ds3_error* error = ds3_get_active_jobs_spectra_s3_request(client, request, &response);

    handle_error(error);
    BOOST_CHECK(response != NULL);
    if (response != NULL) {
        if (!is_live) {
            // Mock mode assertions
            BOOST_CHECK_MESSAGE(response->num_active_jobs == 1, "Mock Active Jobs count should be 1");
            if (response->num_active_jobs > 0) {
                BOOST_CHECK_MESSAGE(strcmp(response->active_jobs[0]->id->value, "5cc5235b-edaf-46b8-ac4d-d45157141668") == 0, "Mock Active Job ID mismatch");
            }
        } else {
            // Live system
            printf("Live Active Jobs count: %lu\n", (unsigned long)response->num_active_jobs);
            uint32_t i;
            for (i = 0; i < response->num_active_jobs; i++) {
                printf("  Active Job ID: %s\n", response->active_jobs[i]->id->value);
            }
        }
        ds3_active_job_list_response_free(response);
    }

    ds3_request_free(request);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(get_active_jobs_xml_jobs_root) {
    ds3_client* client = get_client();
    ds3_client_register_net(client, mock_net_callback_active_jobs_jobs);

    ds3_request* request = ds3_init_get_active_jobs_spectra_s3_request();
    ds3_active_job_list_response* response = NULL;
    ds3_error* error = ds3_get_active_jobs_spectra_s3_request(client, request, &response);

    BOOST_CHECK(error != NULL);
    BOOST_CHECK(error->code == DS3_ERROR_INVALID_XML);
    BOOST_CHECK(strstr(error->message->value, "Expected the root element to be 'Data'") != NULL);

    ds3_error_free(error);
    ds3_request_free(request);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(get_canceled_jobs_xml_data_root) {
    char* endpoint = getenv("DS3_ENDPOINT");
    bool is_live = (endpoint != NULL);
    ds3_client* client = NULL;

    printf("Test get_canceled_jobs_xml_data_root: is_live=%s, endpoint=%s\n", is_live ? "true" : "false", endpoint ? endpoint : "NULL");

    if (!is_live) {
        client = get_client();
        ds3_client_register_net(client, mock_net_callback_canceled_jobs_data);
    } else {
        ds3_error* env_error = ds3_create_client_from_env(&client);
        if (env_error != NULL) {
            handle_error(env_error);
            return;
        }
    }

    ds3_request* request = ds3_init_get_canceled_jobs_spectra_s3_request();
    ds3_canceled_job_list_response* response = NULL;
    ds3_error* error = ds3_get_canceled_jobs_spectra_s3_request(client, request, &response);

    handle_error(error);
    BOOST_CHECK(response != NULL);
    if (response != NULL) {
        if (!is_live) {
            BOOST_CHECK_MESSAGE(response->num_canceled_jobs == 1, "Mock Canceled Jobs count should be 1");
            if (response->num_canceled_jobs > 0) {
                BOOST_CHECK_MESSAGE(strcmp(response->canceled_jobs[0]->id->value, "be40114a-0833-4b12-a4d1-33ea379dae31") == 0, "Mock Canceled Job ID mismatch");
            }
        } else {
            printf("Live Canceled Jobs count: %lu\n", (unsigned long)response->num_canceled_jobs);
            uint32_t i;
            for (i = 0; i < response->num_canceled_jobs; i++) {
                printf("  Canceled Job ID: %s\n", response->canceled_jobs[i]->id->value);
            }
        }
        ds3_canceled_job_list_response_free(response);
    }
    ds3_request_free(request);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(get_completed_jobs_xml_data_root) {
    char* endpoint = getenv("DS3_ENDPOINT");
    bool is_live = (endpoint != NULL);
    ds3_client* client = NULL;

    printf("Test get_completed_jobs_xml_data_root: is_live=%s, endpoint=%s\n", is_live ? "true" : "false", endpoint ? endpoint : "NULL");

    if (!is_live) {
        client = get_client();
        ds3_client_register_net(client, mock_net_callback_completed_jobs_data);
    } else {
        ds3_error* env_error = ds3_create_client_from_env(&client);
        if (env_error != NULL) {
            handle_error(env_error);
            return;
        }
    }

    ds3_request* request = ds3_init_get_completed_jobs_spectra_s3_request();
    ds3_completed_job_list_response* response = NULL;
    ds3_error* error = ds3_get_completed_jobs_spectra_s3_request(client, request, &response);

    handle_error(error);
    BOOST_CHECK(response != NULL);
    if (response != NULL) {
        if (!is_live) {
            BOOST_CHECK_MESSAGE(response->num_completed_jobs == 1, "Mock Completed Jobs count should be 1");
            if (response->num_completed_jobs > 0) {
                BOOST_CHECK_MESSAGE(strcmp(response->completed_jobs[0]->id->value, "e3fe3d7f-2b63-4505-bcc5-38a61eebfdf7") == 0, "Mock Completed Job ID mismatch");
            }
        } else {
            printf("Live Completed Jobs count: %lu\n", (unsigned long)response->num_completed_jobs);
            uint32_t i;
            for (i = 0; i < response->num_completed_jobs; i++) {
                printf("  Completed Job ID: %s\n", response->completed_jobs[i]->id->value);
            }
        }
        ds3_completed_job_list_response_free(response);
    }
    ds3_request_free(request);
    free_client(client);
}

BOOST_AUTO_TEST_CASE(get_jobs_xml_jobs_root) {
    char* endpoint = getenv("DS3_ENDPOINT");
    bool is_live = (endpoint != NULL);
    ds3_client* client = NULL;

    printf("Test get_jobs_xml_jobs_root: is_live=%s, endpoint=%s\n", is_live ? "true" : "false", endpoint ? endpoint : "NULL");

    if (!is_live) {
        client = get_client();
        ds3_client_register_net(client, mock_net_callback_get_jobs_jobs);
    } else {
        ds3_error* env_error = ds3_create_client_from_env(&client);
        if (env_error != NULL) {
            handle_error(env_error);
            return;
        }
    }

    ds3_request* request = ds3_init_get_jobs_spectra_s3_request();
    ds3_job_list_response* response = NULL;
    ds3_error* error = ds3_get_jobs_spectra_s3_request(client, request, &response);

    handle_error(error);
    BOOST_CHECK(response != NULL);
    if (response != NULL) {
        if (!is_live) {
            BOOST_CHECK_MESSAGE(response->num_jobs == 1, "Mock Jobs count should be 1");
            if (response->num_jobs > 0) {
                BOOST_CHECK_MESSAGE(strcmp(response->jobs[0]->job_id->value, "398181ff-fea4-456b-8f3f-50fb8e6c0d87") == 0, "Mock Job ID mismatch");
            }
        } else {
            printf("Live Jobs count: %lu\n", (unsigned long)response->num_jobs);
            uint32_t i;
            for (i = 0; i < response->num_jobs; i++) {
                printf("  Job ID: %s, Bucket: %s\n", response->jobs[i]->job_id->value, response->jobs[i]->bucket_name->value);
            }
        }
        ds3_job_list_response_free(response);
    }
    ds3_request_free(request);
    free_client(client);
}
