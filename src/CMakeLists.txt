
find_package(LibXml2 2.9 REQUIRED)
find_package(CURL 7.31 REQUIRED)
find_package(GLIB2 2.34 REQUIRED)

add_library(ds3 ds3.c)

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

install(TARGETS ds3 DESTINATION lib)
