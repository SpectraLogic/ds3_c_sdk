DS3 C_SDK r1.x to r3.x DS3 API Migration Guide
==============================

All requests have a related ds3_init_* request to configure the request before sending.

Refer to the ds3.h [doxygen comments] (http://spectralogic.github.io/ds3_c_sdk/3.0.0/ds3_8h.html) list the request modifiers that can be applied to each respective request.


API Requests
==============================
| r1.x Request Name | r3.x AWS Request Name | r3.x DS3 Request Name |
|---|---|---|
| ds3_get_system_information | | ds3_get_system_information_spectra_s3_request |
| ds3_verify_system_health | | ds3_verify_system_health_spectra_s3_request |
| ds3_get_bucket | ds3_get_bucket_request | ds3_get_bucket_spectra_s3_request |
| ds3_get_service | | ds3_get_service_request |
| ds3_put_bucket | ds3_put_bucket_request | ds3_put_bucket_spectra_s3_request |
| ds3_put_object | ds3_put_object_request | |
| ds3_delete_folder | | ds3_delete_folder_recursively_spectra_s3_request |
| ds3_bulk | | ds3_put_bulk_job_spectra_s3_request |
| ds3_bulk | | ds3_get_bulk_job_spectra_s3_request |
| ds3_allocate_chunk | | ds3_allocate_job_chunk_spectra_s3_request (*Deprecated in favor of ds3_get_job_chunks_ready_for_client_processing_spectra_s3_request) |
| ds3_get_available_chunks | | ds3_get_job_chunks_ready_for_client_processing_spectra_s3_request |
| ds3_put_object_for_job | | ds3_put_object_request |
| ds3_get_physical_placement | | ds3_get_physical_placement_for_objects_spectra_s3_request |
| ds3_free_available_chunks_response | | ds3_master_object_list_response_free |
| ds3_free_bulk_object_list | | ds3_bulk_object_list_response_free |
| ds3_free_error | | ds3_error_free |
| ds3_free_request | | ds3_request_free |

DS3 API Types
==============================
| r1.x Type Name | r3.x Type Name | r3.x Type Member Name |
|---|---|---|
| ds3_get_service_response | ds3_list_all_my_buckets_result_response | |
| ds3_bulk_object_list | ds3_bulk_object_list_response | |
| ds3_bulk_object | ds3_bulk_object_response | |
| ds3_bulk_response | ds3_master_object_list_response | |
| ds3_get_available_chunks_response | ds3_objects_response| |
| ds3_object | ds3_contents_response | |
| ds3_error->ds3_error_response | *members changed* | |
|   | status_code | http_error_code |
|   | status_message | message |
|   | error_body | code |
|   | *new | resource |
|   | *new |resource_id|
| ds3_get_bucket_response | ds3_list_bucket_result_response | |
| ds3_get_physical_placement_response | ds3_physical_placement_response | |
