cmake_minimum_required(VERSION 2.8)
project(libds3_tests)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../modules/")

#add_executable(test test.cpp metadata_tests.cpp service_tests.cpp search_tests.cpp bucket_tests.cpp deletes_test.cpp job_tests.cpp bulk_get.cpp get_physical_placement.cpp negative_tests.cpp multimap_tests.cpp checksum.cpp)
# checksum.cpp includes sys/mman.h, which is unix only
# multimap_tests.cpp is never built (and wasn't being built)

# some of this stuff really should be gotten from pkg-config
#CFLAGS=`$(PKG_CONFIG) --cflags libds3`
#CFLAGS+= -g -Wall# Debug flags
#CFLAGS+= -DBOOST_TEST_DYN_LINK -lboost_unit_test_framework

add_definitions(-DCURL_STATICLIB)

if (WIN32)

find_package(Boost 1.59.0)
include_directories(${Boost_INCLUDE_DIRS})

include_directories("${PROJECT_SOURCE_DIR}/../src")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/../win32/deps/install/include")
include_directories("${PROJECT_SOURCE_DIR}/../win32/deps/install/include")
include_directories("${PROJECT_SOURCE_DIR}/../win32/deps/install/include/glib-2.0")
include_directories("${PROJECT_SOURCE_DIR}/../win32/deps/install/lib/glib-2.0/include")

link_directories("${PROJECT_SOURCE_DIR}/../win32/output/bin"  "${PROJECT_SOURCE_DIR}/../win32/deps/install/lib")

set(WINDOWS_VS_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${WINDOWS_VS_OUTPUT_DIR}) 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${WINDOWS_VS_OUTPUT_DIR}) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WINDOWS_VS_OUTPUT_DIR})

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${WINDOWS_VS_OUTPUT_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${WINDOWS_VS_OUTPUT_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${WINDOWS_VS_OUTPUT_DIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

else(WIN32)
add_definitions(-DBOOST_TEST_DYN_LINK)
  set(CMAKE_CXX_FLAGS "-g -Wall")

  find_package(GLIB2 2.34 REQUIRED)

  if (GLIB2_FOUND)
    include_directories(${GLIB2_INCLUDE_DIRS})
  endif(GLIB2_FOUND)

link_directories("${PROJECT_SOURCE_DIR}/../install/lib")
include_directories("${PROJECT_SOURCE_DIR}/../install/include")

endif(WIN32)

add_executable(ds3_c_test test.cpp service_tests.cpp checksum.cpp bucket_tests.cpp job_tests.cpp bulk_get.cpp get_physical_placement.cpp negative_tests.cpp multimap_tests.cpp metadata_tests.cpp deletes_test.cpp search_tests.cpp)

target_link_libraries(ds3_c_test ds3 glib-2.0 boost_unit_test_framework)

#test.cpp metadata_tests.cpp service_tests.cpp search_tests.cpp bucket_tests.cpp deletes_test.cpp job_tests.cpp bulk_get.cpp get_physical_placement.cpp negative_tests.cpp multimap_tests.cpp checksum.cpp 	
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/")

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/_build/binaries)
