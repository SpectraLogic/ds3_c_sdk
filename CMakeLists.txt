cmake_minimum_required(VERSION 2.8)
project(libds3)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/")

set (DS3SDK_VERSION_MAJOR 4)
set (DS3SDK_VERSION_MINOR 1)
set (DS3SDK_VERSION_PATCH 0)

add_subdirectory(src)
