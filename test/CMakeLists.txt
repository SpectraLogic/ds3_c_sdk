cmake_minimum_required(VERSION 2.8)
project(libds3_tests)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../modules/")

#add_executable(test test.cpp metadata_tests.cpp service_tests.cpp search_tests.cpp bucket_tests.cpp deletes_test.cpp job_tests.cpp bulk_get.cpp get_physical_placement.cpp negative_tests.cpp multimap_tests.cpp checksum.cpp)
add_executable(test test.cpp checksum.cpp service_tests.cpp bucket_tests.cpp job_tests.cpp bulk_get.cpp get_physical_placement.cpp negative_tests.cpp metadata_tests.cpp deletes_test.cpp search_tests.cpp)

# some of this stuff really should be gotten from pkg-config
#CFLAGS=`$(PKG_CONFIG) --cflags libds3`
#CFLAGS+= -g -Wall# Debug flags
#CFLAGS+= -DBOOST_TEST_DYN_LINK -lboost_unit_test_framework
add_definitions(-DBOOST_TEST_DYN_LINK)

if (WIN32)
else(WIN32)
  set(CMAKE_CXX_FLAGS "-g -Wall")
  
  find_package(GLIB2 2.34 REQUIRED)

  if (GLIB2_FOUND)
    include_directories(${GLIB2_INCLUDE_DIRS})
  endif(GLIB2_FOUND)
endif(WIN32)

link_directories("${PROJECT_SOURCE_DIR}/../install/lib")
target_link_libraries(test ds3 glib-2.0 boost_unit_test_framework)

include_directories("${PROJECT_SOURCE_DIR}/../install/include")

#test.cpp metadata_tests.cpp service_tests.cpp search_tests.cpp bucket_tests.cpp deletes_test.cpp job_tests.cpp bulk_get.cpp get_physical_placement.cpp negative_tests.cpp multimap_tests.cpp checksum.cpp 	
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/")

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/_build/binaries)