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
    #set(WINDOWS_VS_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/win32/src/Debug)
    set(WINDOWS_VS_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/win32/src/Release)

    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${WINDOWS_VS_OUTPUT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${WINDOWS_VS_OUTPUT_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WINDOWS_VS_OUTPUT_DIR})

    foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
        string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
        set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${WINDOWS_VS_OUTPUT_DIR} )
        set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${WINDOWS_VS_OUTPUT_DIR} )
        set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${WINDOWS_VS_OUTPUT_DIR} )
    endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

    add_definitions(-DLIBRARY_EXPORTS -DCURL_STATICLIB -Dinline=__inline -D__STDC_FORMAT_MACROS -D_CRT_SECURE_NO_WARNINGS)
    set_source_files_properties(ds3.c PROPERTIES LANGUAGE CXX)

    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/win32" WINDOWS_DIR)
    file(TO_NATIVE_PATH "${WINDOWS_DIR}/deps/install" WINDOWS_INSTALL_DIR)
    file(TO_NATIVE_PATH "${WINDOWS_INSTALL_DIR}/lib" WINDOWS_INSTALL_LIB_DIR)
    file(TO_NATIVE_PATH "${WINDOWS_INSTALL_DIR}/include" WINDOWS_INSTALL_INCLUDE_DIR)

    include_directories(
        ${WINDOWS_INSTALL_INCLUDE_DIR}
        ${WINDOWS_INSTALL_INCLUDE_DIR}/glib-2.0
        ${WINDOWS_INSTALL_INCLUDE_DIR}/glib-2.0/glib
        ${WINDOWS_INSTALL_LIB_DIR}/glib-2.0/include
        ${WINDOWS_DIR}/compatibility_includes)

    target_link_libraries(ds3
        ${WINDOWS_INSTALL_LIB_DIR}/libcurl_a.lib
        ${WINDOWS_INSTALL_LIB_DIR}/zlib_a.lib
        ${WINDOWS_INSTALL_LIB_DIR}/libxml2_a.lib
        ${WINDOWS_INSTALL_LIB_DIR}/glib-2.0.lib
        Ws2_32)

    file(TO_NATIVE_PATH "${WINDOWS_VS_OUTPUT_DIR}/ds3.dll" DLL_LOCATION)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/test/bin" UNIT_TEST_DIR)
    add_custom_command(TARGET ds3 POST_BUILD
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
    set(CMAKE_BUILD_TYPE Release)
    #set(CMAKE_BUILD_TYPE Debug)
endif(WIN32)
