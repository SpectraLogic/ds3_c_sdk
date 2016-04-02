/*
 * ******************************************************************************
 *   Copyright 2014-2015 Spectra Logic Corporation. All Rights Reserved.
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

/*
 * Set environment vars to match target device, e.g.:
     export DS3_ACCESS_KEY="ams="
    export DS3_SECRET_KEY="tzcWTTYE"
    export DS3_ENDPOINT="http://192.168.1.20:8080"
    // http_proxy is optional
    export http_proxy="http://10.0.2.2:8888"
*/

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
