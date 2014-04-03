#include <stdlib.h>
#include <stdio.h>
#include "./ds3.h"

int main (int args, char * argv[]) {
  printf("Starting playing with sdk code\n");

  ds3_request * request = ds3_init_get_service();

  ds3_print_request(request);

  ds3_free_request(request);
}
