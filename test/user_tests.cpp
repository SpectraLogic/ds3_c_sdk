#include <stdio.h>
#include <glib.h>
#include "ds3.h"
#include "test.h"
#include <boost/test/unit_test.hpp>

ds3_bool has_spectra_user(const ds3_spectra_user_list_response* users_list, const char* search_user_name) {
    if (users_list == NULL) {
        return False;
    }

    for (size_t user_index; user_index < users_list->num_spectra_users; user_index++) {
        if (g_strcmp0(search_user_name, users_list->spectra_users[user_index]->name->value) == 0) {
            return True;
        }
    }


    return False;
}

BOOST_AUTO_TEST_CASE( get_users ) {
    ds3_client* client = get_client();
    const char* bucket_name = "get_users_test_bucket";
    ds3_spectra_user_list_response* response;

    populate_with_objects(client, bucket_name);

    ds3_request* request = ds3_init_get_users_spectra_s3_request();
    ds3_request_set_page_offset(request, 0)

    ds3_error* error = ds3_get_users_spectra_s3_request(client, request, &response);
    handle_error(error);
    ds3_request_free(request);

    BOOST_CHECK(response->num_spectra_users, 5);
    BOOST_CHECK(response->paging != NULL);
    BOOST_CHECK(response->paging->page_truncated == 0);
    BOOST_CHECK(response->paging->total_result_count == 5);

    ds3_s3_object_list_response_free(response);

    clear_bucket(client, bucket_name);
    free_client(client);
}

