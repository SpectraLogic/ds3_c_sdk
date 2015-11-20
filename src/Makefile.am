lib_LTLIBRARIES = libds3.la
include_HEADERS = ds3.h ds3_string_multimap.h ds3_net.h ds3_request.h ds3_utils.h

libds3_la_SOURCES = ds3.c ds3_string_multimap.c ds3_net.c
libds3_la_CFLAGS = $(LIBXML2_CFLAGS) $(GLIB2_CFLAGS) $(LIBCURL_CFLAGS) -Wall
libds3_la_LIBADD = $(LIBXML2_LIBS) $(GLIB2_LIBS) $(LIBCURL_LIBS)
libds3_la_LDFLAGS = -version-info 0:0:0

