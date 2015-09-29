add_library(ds3 SHARED ds3.c ds3.h)
if (WIN32)

add_definitions(-DLIBRARY_EXPORTS -DCURL_STATICLIB -Dinline=__inline)
SET_SOURCE_FILES_PROPERTIES(ds3.c PROPERTIES LANGUAGE CXX)

include_directories(../win32/deps/install/include ../win32/deps/install/include/glib-2.0 ../win32/deps/install/lib/glib-2.0/include ../win32/compatibility_includes)
#target_link_libraries(ds3 ../win32/deps/install/bin)
target_link_libraries(ds3 ../win32/deps/install/lib/glib-2.0 ../win32/deps/install/lib/libxml2_a ../win32/deps/install/lib/libcurl_a ../win32/deps/install/lib/zlib_a)

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

endif(WIN32)
install(TARGETS ds3 DESTINATION lib)
