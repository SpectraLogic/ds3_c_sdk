cmake_minimum_required(VERSION 2.8)
project(libds3)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/")

set (DS3SDK_VERSION_MAJOR 3)
set (DS3SDK_VERSION_MINOR 2)
set (DS3SDK_VERSION_PATCH 0)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/_build/binaries)

add_subdirectory(src)
