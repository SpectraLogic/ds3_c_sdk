if (WIN32)
set(WINDOWS_VS_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/win32/output/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${WINDOWS_VS_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${WINDOWS_VS_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WINDOWS_VS_OUTPUT_DIR})

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${WINDOWS_VS_OUTPUT_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${WINDOWS_VS_OUTPUT_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${WINDOWS_VS_OUTPUT_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endif(WIN32)

add_library(ds3 SHARED
            ds3.h ds3.c
            ds3_net.h ds3_net.c
            ds3_request.h
            ds3_string_multimap_impl.h ds3_string_multimap_impl.c
            ds3_string_multimap.h ds3_string_multimap.c
            ds3_string.h ds3_string.c
            ds3_utils.h ds3_utils.c
            ds3_connection.h ds3_connection.c)

if (WIN32)

    add_definitions(-DLIBRARY_EXPORTS -DCURL_STATICLIB -Dinline=__inline -D__STDC_FORMAT_MACROS -D_CRT_SECURE_NO_WARNINGS)
    set_source_files_properties(ds3.c PROPERTIES LANGUAGE CXX)

include_directories(
    ../win32/deps/install/include
    ../win32/deps/install/include/glib-2.0
    ../win32/deps/install/lib/glib-2.0/include
    ../win32/compatibility_includes)
target_link_libraries(ds3
    ../win32/deps/install/lib/libcurl_a
    ../win32/deps/install/lib/zlib_a
    ../win32/deps/install/lib/libxml2_a
    ../win32/deps/install/lib/glib-2.0
    Ws2_32)

file(TO_NATIVE_PATH ${WINDOWS_VS_OUTPUT_DIR}/ds3.dll DLL_LOCATION)
file(TO_NATIVE_PATH ${PROJECT_SOURCE_DIR}/test/bin UNIT_TEST_DIR)
ADD_CUSTOM_COMMAND(TARGET ds3 POST_BUILD
                   COMMAND copy
                   ${DLL_LOCATION} ${UNIT_TEST_DIR})
else(WIN32)
find_package(LibXml2 2.9 REQUIRED)
find_package(CURL 7.31 REQUIRED)
find_package(GLIB2 2.34 REQUIRED)

if (LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIR})
  target_link_libraries (ds3 ${LIBXML2_LIBRARIES})
endif (LIBXML2_FOUND)

if (CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIRS})
  target_link_libraries (ds3 ${CURL_LIBRARIES})
endif (CURL_FOUND)

if (GLIB2_FOUND)
  include_directories(${GLIB2_INCLUDE_DIRS})
  target_link_libraries (ds3 ${GLIB2_LIBRARIES})
endif(GLIB2_FOUND)

install(FILES
          "ds3.h"
          "ds3_connection.h"
          "ds3_net.h"
          "ds3_request.h"
          "ds3_string.h"
          "ds3_string_multimap.h"
          "ds3_string_multimap_impl.h"
          "ds3_utils.h"
        DESTINATION
          "/usr/local/include")
install(TARGETS ds3 DESTINATION lib)
endif(WIN32)
