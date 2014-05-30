AC_INIT([ds3], [1.0], [SDKDeveloper@spectralogic.com])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC

AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_PROG_AR
AM_PROG_CC_C_O

LT_INIT

PKG_CHECK_MODULES([GLIB2], [glib-2.0 >= 2.36])
PKG_CHECK_MODULES([LIBCURL], [libcurl >= 7.31])
PKG_CHECK_MODULES([LIBXML2], [libxml-2.0 >= 2.9])

LIBDS3_DEPENDENCIES="glib-2.0 >= 2.36 libcurl >= 7.31 libxml-2.0 >= 2.9"

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile libds3.pc])
AC_OUTPUT

