cmake_minimum_required(VERSION 2.8)
project(libds3_tests)

enable_testing()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../modules/")


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

else(WIN32) # POSIX
  add_definitions(-DBOOST_TEST_DYN_LINK)
  set(CMAKE_CXX_FLAGS "-g -Wall")

  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GLIB2 glib-2.0>=2.34)

  if (GLIB2_FOUND)
    message(STATUS "Found glib-2.0")
    include_directories(${GLIB2_INCLUDE_DIRS})
  endif(GLIB2_FOUND)

  find_path(GLIB_CONFIG_INCLUDE_DIR
    NAMES glibconfig.h
    PATH_SUFFIXES glib-2.0/include
    PATHS ${GLIB2_INCLUDE_DIRS})

  if (GLIB_CONFIG_INCLUDE_DIR)
    SET ( GLIB2_INCLUDE_DIRS ${GLIB2_INCLUDE_DIRS} ${GLIB_CONFIG_INCLUDE_DIR} )
  else (GLIB_CONFIG_INCLUDE_DIR)
    message(STATUS "Could not find glibconfig.h in ${GLIB2_INCLUDE_DIRS.")
  endif (GLIB_CONFIG_INCLUDE_DIR)

  link_directories("${PROJECT_SOURCE_DIR}/../install/lib")
  include_directories("${PROJECT_SOURCE_DIR}/../install/include")
endif(WIN32)

add_executable(ds3_c_test
  bucket_tests.cpp
  bulk_get.cpp
  bulk_put.cpp
  checksum.cpp
  deletes_test.cpp
  get_physical_placement.cpp
  job_tests.cpp
  metadata_tests.cpp
  multimap_tests.cpp
  negative_tests.cpp
  search_tests.cpp
  service_tests.cpp
  test.cpp)

add_test(regression_tests ds3_c_test)

target_link_libraries(ds3_c_test ds3 glib-2.0 boost_unit_test_framework)

