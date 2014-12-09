DS3 C SDK
=========

This project contains a C library for using the DS3 Deep Storage REST interface.

Contact Us
==========

Join us at our [Google Groups](https://groups.google.com/d/forum/spectralogicds3-sdks) forum to ask questions, or see frequently asked questions.

Windows
=======

For Windows we release a binary version of the SDK for your convenience. See
[win32/README.dm](win32/README.md) for more information about building from
source in Windows.

Release Zip
-----------

The zip file available on the Releases tab on the github page contains these
files:

    ds3\bin\ds3.dll
    ds3\bin\ds3.dll.manifest
    ds3\bin\libglib-2.0-0.dll
    ds3\bin\libiconv-2.dll
    ds3\bin\libintl-8.dll
    ds3\bin\pthreadGC2.dll
    ds3\include\ds3.h
    ds3\lib\ds3.exp
    ds3\lib\ds3.lib

To compile against the library:

1. Include ds3\include\ds3.h
2. Link against ds3\lib\ds3.lib
3. Distribute ds3\bin\*.dll along with your application.

Unix/Linux
==========

For Unix/Linux we distribute the SDK as source code. The release tarballs
contain a simple build script that should work on most Unix/Linux systems.

The SDK depends upon several open source libraries, so you'll need to ensure
that you've installed the development header packages for each of them. For
example, Linux systems often provide lib\*-dev or lib\*-devel packages. The DS3
dependencies are:

* libxml2
* libcurl
* libglib-2.0

Release Tarball
---------------

Download the latest tarball from the Releases tab on the github page and open a
terminal window.

    $ cd directory/containing/release/tarball
    $ tar zxf ds3_c_sdk-{version}.tgz
    $ cd ds3_c_sdk-{version}
    $ ./configure
    $ make
    $ su
    # make install
    # ldconfig
    # exit
    $

The `make install` command installs the following files on your system:

    {prefix}/lib/libds3.la
    {prefix}/lib/libds3.a
    {prefix}/lib/pkgconfig/libds3.pc
    {prefix}/lib/libds3.so.0.0.0
    {prefix}/include/ds3.h

The `./configure` command sets `{prefix}` to `/usr/local` by default. You can
change this by running `./configure --prefix=/your/desired/prefix` instead of
`./configure`.

GIT Clone
---------

To build the SDK from a git clone you must have a relatively recent version of
GNU autoutils installed. While the release tarball does contain the build
scripts that autoutils generates, the git repository does not.

Assuming you have autotools installed, you can execute `autoreconf --install`
and then follow the same instructions as the tarball release.

Usage
-----

The SDK provides a pkg-config definition, so you can determine the proper
compiler tags using the following commands:

 * Compiler flags: `pkg-config --cflags libds3`
 * Linker flags: `pkg-config --libs libds3`
 * Both compiler and linker flags: `pkg-config --cflags --libs libds3`

Sample
------

The `/sample` directory has an example project that uses the sdk. It provides a Makefile
to build the SDK and itself.

To build the sample, use the following commands:

    $ cd directory/containing/source/tree
    $ cd sample
    $ make deps # Builds the SDK and installs it into directory/containing/source/tree**/install**
    $ make

To run it, just use `make run`.

Code Samples
------------

The following section contains several examples of using the DS3 C SDK.  The first example shows how to get a list of all the buckets back from DS3:

```c

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
// "ds3.h" is the only header that needs to be included to use the DS3 API.
#include "ds3.h"

int main (int args, char * argv[]) {
    // Allocate space for the response pointer.
    // The DS3 client library will end up consuming this.
    ds3_get_service_response *response; 
    uint64_t i;

    // Setup client credentials and then the actual client itself.
    ds3_creds * creds = ds3_create_creds("cnlhbg==","ZIjGDQAs");
    ds3_client * client = ds3_create_client("http://192.168.56.101:8080", creds);
    
    // You can optionally set a proxy server that a request should be sent through
    //ds3_client_proxy(client, "192.168.56.1:8888");
    
    // Create the get service request.  All requests to a DS3 appliance start this way.
    // All ds3_init_* functions return a ds3_request struct
    
    ds3_request* request = ds3_init_get_service();
    
    // This performs the request to a DS3 appliance.
    // If there is an error 'error' will not be NULL
    // If the request completed successfully then 'error' will be NULL
    ds3_error* error = ds3_get_service(client, request, &response);
    ds3_free_request(request);
   
    // Check that the request completed successfully
    if(error != NULL) {
        if(error->error != NULL) {
            printf("Got an error (%lu): %s\n", error->error->status_code, error->message);
            printf("Message Body: %s\n", error->error->error_body);
        }
        else {
            printf("Got a runtime error: %s\n", error->message);
        }
        ds3_free_error(error);
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }

    if (response == NULL) {
        printf("Response was null\n");
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }

    if(response->num_buckets == 0) {
        printf("No buckets returned\n");
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 0;
    }

    for (i = 0; i < response->num_buckets; i++) {
        ds3_bucket bucket = response->buckets[i];
        printf("Bucket: (%s) created on %s\n", bucket.name, bucket.creation_date);
    }
    
    ds3_free_service_response(response);

    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_cleanup();
    
    return 0;
}

```

The next demonstrates how to create a new bucket:

```c

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ds3.h"

int main (int args, char * argv[]) {
    ds3_creds * creds = ds3_create_creds("cnlhbg==","ZIjGDQAs");
    ds3_client * client = ds3_create_client("http://192.168.56.101:8080", creds);
    
    char * bucket = "books";
    ds3_request * create_bucket_request = ds3_init_put_bucket(bucket);
    ds3_error* error = ds3_put_bucket(client, create_bucket_request);
    ds3_free_request(create_bucket_request);
    
    if(error != NULL) {
        if(error->error != NULL) {
            printf("Failed to create bucket with error (%lu): %s\n", error->error->status_code, error->message);
            printf("Message Body: %s\n", error->error->error_body);
        }
        else {
            printf("Got a runtime error: %s\n", error->message);
        }
        ds3_free_error(error);
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }   

    printf("Successfully created bucket: %s\n", bucket);
    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_cleanup();

    return 0;
}
```

The following is an example of performing a get bucket:

```c

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
// "ds3.h" is the only header that needs to be included to use the DS3 API.
#include "ds3.h"

int main (int args, char * argv[]) {
    ds3_get_bucket_response *response; 
    uint64_t i;

    // Setup client credentials and then the actual client itself.
    ds3_creds * creds = ds3_create_creds("cnlhbg==","ZIjGDQAs");
    ds3_client * client = ds3_create_client("http://192.168.56.101:8080", creds);

    // You can optionally set a proxy server that a request should be sent through
    //ds3_client_proxy(client, "192.168.56.1:8888");

    char * bucket = "books";

    // Create the get bucket request.
    ds3_request* request = ds3_init_get_bucket(bucket);

    // This performs the request to a DS3 appliance.
    // If there is an error 'error' will not be NULL
    // If the request completed successfully then 'error' will be NULL
    ds3_error* error = ds3_get_bucket(client, request, &response);
    ds3_free_request(request);

    // Check that the request completed successfully
    if(error != NULL) {
        if(error->error != NULL) {
            printf("Got an error (%lu): %s\n", error->error->status_code, error->message);
            printf("Message Body: %s\n", error->error->error_body);
        }
        else {
            printf("Got a runtime error: %s\n", error->message);
        }
        ds3_free_error(error);
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }

    if (response == NULL) {
        printf("Response was null\n");
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 1;
    }

    if(response->num_objects == 0) {
        printf("No objects returned\n");
        ds3_free_bucket_response(response);
        ds3_free_creds(creds);
        ds3_free_client(client);
        return 0;
    }

    for (i = 0; i < response->num_objects; i++) {
        ds3_object object = response->objects[i];
        printf("Object: (%s) created on %s\n", object.name, object.last_modified);
    }

    ds3_free_bucket_response(response);

    ds3_free_creds(creds);
    ds3_free_client(client);
    ds3_cleanup();
    return 0;    
}
```

The structure of the code is very similar to the previous examples.  Setup the client, setup the call, perform the call.  Every request follows this same pattern.  Things get a little more complicated with the bulk get/put cases.  The following bulk put will demonstrate some of those complexities

