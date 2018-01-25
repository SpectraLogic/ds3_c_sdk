Spectra S3 C SDK
================

[![Apache V2 License](http://img.shields.io/badge/license-Apache%20V2-blue.svg)](https://github.com/SpectraLogic/ds3_c_sdk/blob/master/LICENSE.md)

This project contains a C library for using the Spectra S3 Deep Storage REST interface.

Contact Us
==========

Join us at our [Google Groups](https://groups.google.com/d/forum/spectralogicds3-sdks) forum to ask questions, or see frequently asked questions.

Contribute
==========

Pull requests are welcome, but first sign the [Contributor's
Agreement](https://developer.spectralogic.com/contributors-agreement/)

Windows
=======

For Windows we release a binary version of the SDK for your convenience. See
[win32/README.md](win32/README.md) for more information about building from
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
    ds3\include\ds3_string.h
    ds3\include\ds3_string_multimap.h
    ds3\lib\ds3.exp
    ds3\lib\ds3.lib

To compile against the library:

1. Include ds3\include\ds3.h
2. Link against ds3\lib\ds3.lib
3. Distribute ds3\bin\*.dll along with your application.

Unix/Linux
==========

For Unix/Linux we distribute the SDK as source code. The release tarballs
contain a simple build script that should work on most Unix/Linux systems.  The
build system is currently CMake.

To install CMake on Ubuntu use apt-get and install the following:

    $ sudo apt-get install cmake

To install CMake on CentOS use yum and install the following:
    $ sudo yum install cmake

The SDK depends upon several open source libraries, so you'll need to ensure
that you've installed the development header packages for each of them. For
example, Linux systems often provide lib\*-dev or lib\*-devel packages. The Spectra S3
dependencies are:

* libxml2
* libcurl
* libglib-2.0

On Ubuntu you can install them with apt-get:

    $ sudo apt-get install libxml2-dev
    $ sudo apt-get install libcurl4-openssl-dev
    $ sudo apt-get install libglib2.0-dev

On CentOS you can install them with yum:

    $ sudo yum install libxml2-devel
    $ sudo yum install libcurl-devel
    $ sudo yum install glib2-devel

For testing you will need the boost unit test library as well.

On Ubuntu this can be installed with:

    $ sudo apt-get install libboost-test-dev

On CentOS this can be installed with:

    $ sudo yum install boost-test


RPM build on CentOS/7
=====================

Build from cloned source repository:

    $ ./rpmbuild.sh

The command will produce both src.rpm and and binary rpm.

Content of the compiled package:

    $ rpm -qlp /home/yu/rpmbuild/RPMS/x86_64/ds3_c_sdk-3.2.3-11.el7.centos.x86_64.rpm
    /usr/include/ds3/ds3.h
    /usr/include/ds3/ds3_connection.h
    /usr/include/ds3/ds3_net.h
    /usr/include/ds3/ds3_request.h
    /usr/include/ds3/ds3_string.h
    /usr/include/ds3/ds3_string_multimap.h
    /usr/include/ds3/ds3_string_multimap_impl.h
    /usr/include/ds3/ds3_utils.h
    /usr/lib64/libds3.so
    /usr/share/doc/ds3_c_sdk-3.2.3/LICENSE.md
    /usr/share/doc/ds3_c_sdk-3.2.3/README.md
    /usr/share/doc/ds3_c_sdk-3.2.3/r3x_migration_guide.md


Release Tarball
---------------

Download the latest tarball from the Releases tab on the github page and open a
terminal window.

    $ cd directory/containing/release/tarball
    $ tar zxf ds3_c_sdk-{version}.tgz
    $ cd ds3_c_sdk-{version}
    $ cmake .
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

GIT Clone
---------

To build the SDK from a git clone you must have a relatively recent version of
GNU CMake installed. While the release tarball does contain the build
scripts that CMake generates, the git repository does not.

Assuming you have CMake installed, you can follow the same instructions as the tarball release.

pkg-config Usage
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

To run it, first ensure that DS3_ACCESS_KEY, DS3_SECRET_KEY, DS3_ENDPOINT (and optionally http:proxy) are set in environment variables to match the target device. For the simulator, see [Installation Instructions] (https://developer.spectralogic.com/sim-install/) 

    $ make run-put-bulk     # create "books" bucket and put files into it
    $ make run-get-service  # list all buckets
    $ make run-get-bucket   # list contents of "books" bucket
    $ make run-get-object   # get first book and write to temp file

Tests
-----

The `/test` directory contains function tests of the sdk.  Ensure that the environment variables DS3_ACCESS_KEY, DS3_SECRET_KEY, DS3_ENDPOINT (and optionally http:proxy) are set. To build and run the tests, use the following commands:

    $ cd directory/containing/source/tree
    $ cmake .
    $ make
    $ make install
    $ ldconfig

    $ cd test
    $ cmake .
    $ make
    $ make test


Documentation
-------------
For the list of API calls in the C SDK please see the documentation [here](http://spectralogic.github.io/ds3_c_sdk/index.html)

Code Samples
------------

The following section contains several examples of using the DS3 C SDK.  The first example shows how to get a list of all the buckets back from Spectra S3:

```c
#include <stdlib.h>
#include <stdio.h>
// "ds3.h" is the only header that needs to be included to use the DS3 API.
#include "ds3.h"

int main (void) {
    // Get Service
    ds3_client* client;
    ds3_request* request;
    ds3_error* error;
    ds3_list_all_my_buckets_result_response *response;
    uint64_t bucket_index;

    // Create a client from environment variables
    error = ds3_create_client_from_env(&client);
    handle_error(error);

    // Create the get service request.  All requests to a DS3 appliance start this way.
    // All ds3_init_* functions return a ds3_request struct
    request = ds3_init_get_service_request();

    // This performs the request to a DS3 appliance.
    // If there is an error 'error' will not be NULL
    // If the request completed successfully then 'error' will be NULL
    error = ds3_get_service_request(client, request, &response);
    ds3_request_free(request);
    handle_error(error);

    if(response->num_buckets == 0) {
        printf("No buckets returned\n");
        ds3_list_all_my_buckets_result_response_free(response);
        ds3_creds_free(client->creds);
        ds3_client_free(client);
        return 0;
    }

    for (bucket_index = 0; bucket_index < response->num_buckets; bucket_index++) {
        ds3_bucket_details_response* bucket = response->buckets[bucket_index];
        printf("Bucket: (%s) created on %s\n", bucket->name->value, bucket->creation_date->value);
    }

    ds3_list_all_my_buckets_result_response_free(response);
    ds3_creds_free(client->creds);
    ds3_client_free(client);
    ds3_cleanup();

    return 0;
}


```

The next demonstrates how to create a new bucket:

```c

#include <stdlib.h>
#include <stdio.h>
// "ds3.h" is the only header that needs to be included to use the DS3 API.
#include "ds3.h"

int main (int args, char * argv[]) {
    ds3_creds * creds = ds3_create_creds("cnlhbg==","ZIjGDQAs");
    ds3_client * client = ds3_create_client("http://192.168.56.101:8080", creds);

    char * bucket = "books";
    ds3_request * create_bucket_request = ds3_init_put_bucket_request(bucket_name);
    ds3_error* error = ds3_put_bucket_request(client, create_bucket_request);
    ds3_request_free(create_bucket_request);

    if(error != NULL) {
        if(error->error != NULL) {
            printf("Failed to create bucket with error (%lu): %s\n", error->error->http_error_code, error->message->value);
            printf("Message Body: %s\n", error->error->message->value);
            printf("Resource: %s\n", error->error->resource->value);
        }
        else {
            printf("Got a runtime error: %s\n", error->message->value);
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
#include <stdio.h>
// "ds3.h" is the only header that needs to be included to use the DS3 API.
#include "ds3.h"

int main (int args, char * argv[]) {
    ds3_list_bucket_result_response* response;
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
    ds3_error* error = ds3_get_bucket_request(client, request, &response);
    ds3_request_free(request);

    // Check that the request completed successfully
    if(error != NULL) {
        if(error->error != NULL) {
            printf("Failed to create bucket with error (%lu): %s\n", error->error->http_error_code, error->message->value);
            printf("Message Body: %s\n", error->error->message->value);
            printf("Resource: %s\n", error->error->resource->value);
        }
        else {
            printf("Got a runtime error: %s\n", error->message->value);
        }
        ds3_list_bucket_result_response_free(response);
        ds3_error_free(error);
        ds3_creds_free(creds);
        ds3_client_free(client);
        return 1;
    }

    if (response == NULL) {
        printf("Response was null\n");
        ds3_creds_free(creds);
        ds3_client_free(client);
        return 1;
    }

    if(response->num_objects == 0) {
        printf("No objects returned\n");
        ds3_list_bucket_result_response_free(response);
        ds3_creds_free(creds);
        ds3_client_free(client);
        return 0;
    }

    for (i = 0; i < response->num_objects; i++) {
        ds3_contents_response* object = response->objects[i];
        printf("Object: (%s) created on %s\n", object->name->value, object->last_modified->value);
    }

    ds3_list_bucket_result_response_free(response);

    ds3_creds_free(creds);
    ds3_client_free(client);
    ds3_cleanup();
    return 0;
}
```

The structure of the code is very similar to the previous examples.  Setup the client, setup the call, perform the call.  Every request follows this same pattern.

Additional examples are available here: [samples](sample)
