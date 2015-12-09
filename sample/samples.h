/*** Shared header for all Samples  ***/
/*** Spectra Logic December 9, 2015 ***/

#define HTTP_PROXY "10.0.2.2:8888"
#define DS3_ENDPOINT "http://192.168.1.20:8080"
#define DS3_SECRET_KEY "tzcWTTYE"
#define DS3_ACCESS_KEY  "ams="

#define BUCKETNAME ((const char *)"books")
#define BOOKS {"resources/beowulf.txt", "resources/sherlock_holmes.txt", "resources/tale_of_two_cities.txt", "resources/ulysses.txt"}

/*
* Prints the contents of an error to stdout
*/
inline void print_error(const ds3_error* error) {
      printf("ds3_error_message: %s\n", error->message->value);
      if (error->error != NULL) {
          printf("ds3_status_code: %lu\n", (long unsigned int)error->error->status_code);
          printf("ds3_status_message: %s\n", error->error->status_message->value);
          printf("ds3_error_body: %s\n", error->error->error_body->value);
      }
}

/*
* Prints an error if it is not null and exits the process with return code 1
*/ 
inline void handle_error(ds3_error* error) {
    if (error != NULL) {
        print_error(error);
        ds3_free_error(error);
        exit(1);
    }
}
