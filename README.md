DS3 C SDK
=========

This project contains a C library for using the DS3 Deep Storage REST interface.

Windows
=======

For Windows we release a binary version of the SDK for your convenience. See
[win32/README.dm](win32/README.md) for more information about building from
source in Windows.

Release Zip
-----------

The zip file available on the Releases tab on the github page contains these
files:

    ds3\bin\ds3.dll
    ds3\bin\ds3.dll.manifest
    ds3\bin\libglib-2.0-0.dll
    ds3\bin\libiconv-2.dll
    ds3\bin\libintl-8.dll
    ds3\bin\pthreadGC2.dll
    ds3\include\ds3.h
    ds3\lib\ds3.exp
    ds3\lib\ds3.lib

To compile against the library:

1. Include ds3\include\ds3.h
2. Link against ds3\lib\ds3.lib
3. Distribute ds3\bin\*.dll along with your application.

Unix/Linux
==========

For Unix/Linux we distribute the SDK as source code. The release tarballs
contain a simple build script that should work on most Unix/Linux systems.

The SDK depends upon several open source libraries, so you'll need to ensure
that you've installed the development header packages for each of them. For
example, Linux systems often provide lib\*-dev or lib\*-devel packages. The DS3
dependencies are:

* libxml2
* libcurl
* libglib-2.0

Release Tarball
---------------

Download the latest tarball from the Releases tab on the github page and open a
terminal window.

    $ cd directory/containing/release/tarball
    $ tar zxf ds3_c_sdk-{version}.tgz
    $ cd ds3_c_sdk-{version}
    $ ./configure
    $ make
    $ su
    # make install
    # ldconfig
    # exit
    $

The `make install` command installs the following files on your system:

    {prefix}/lib/libds3.la
    {prefix}/lib/libds3.a
    {prefix}/lib/pkgconfig/libds3.pc
    {prefix}/lib/libds3.so.0.0.0
    {prefix}/include/ds3.h

The `./configure` command sets `{prefix}` to `/usr/local` by default. You can
change this by running `./configure --prefix=/your/desired/prefix` instead of
`./configure`.

GIT Clone
---------

To build the SDK from a git clone you must have a relatively recent version of
GNU autoutils installed. While the release tarball does contain the build
scripts that autoutils generates, the git repository does not.

Assuming you have autotools installed, you can execute `autoreconf --install`
and then follow the same instructions as the tarball release.

Usage
-----

The SDK provides a pkg-config definition, so you can determine the proper
compiler tags using the following commands:

 * Compiler flags: `pkg-config --cflags libds3`
 * Linker flags: `pkg-config --libs libds3`
 * Both compiler and linker flags: `pkg-config --cflags --libs libds3`

# Sample

The `/sample` directory has an example of using the sdk. It provides a Makefile
to build the SDK and itself.

To build the sample, use the following commands:

    $ cd directory/containing/source/tree
    $ cd sample
    $ make deps # Builds the SDK and installs it into directory/containing/source/tree**/install**
    $ make

To run it, just use `make run`.

