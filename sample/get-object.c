#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ds3.h"

void print_error(const ds3_error* error) {
      printf("ds3_error_message: %s\n", error->message->value);
      if (error->error != NULL) {
          printf("ds3_status_code: %lu\n", error->error->status_code);
          printf("ds3_status_message: %s\n", error->error->status_message->value);
          printf("ds3_error_body: %s\n", error->error->error_body->value);
      }
}

int main(void) {

    // Setup client credentials and then the actual client itself.
    ds3_creds * creds = ds3_create_creds("cnlhbg==","ZIjGDQAs");
    ds3_client * client = ds3_create_client("http://localhost:8080", creds);

    char * bucket = "books";
    ds3_request* request = ds3_init_get_object_for_job(bucket, "obj", 0, NULL);

    FILE* tmp_file = tmpfile();
    ds3_error* error = ds3_get_object(client, request, tmp_file, ds3_write_to_file);

    ds3_free_request(request);
    fclose(tmp_file);

    if (error != NULL) {
        fprintf(stdout, "Got an error\n");
        print_error(error);
        ds3_free_error(error);
    }

    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_cleanup();

    return 0;
}
