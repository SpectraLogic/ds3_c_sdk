DS3 C SDK
=========

This project contains a C library for using the DS3 Deep Storage REST interface.

Installation
============

Windows
-------

TODO: We're still working on this. Stay tuned.

Unix Release Tarball
--------------------

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

Unix GIT Clone
--------------

To build the SDK from a git clone you must have a relatively recent version of
GNU autoutils installed. While the release tarball does contain the build
scripts that autoutils generates, the git repository does not.

Assuming you have autotools installed, you can execute `autoreconf --install`
and then follow the same instructions as the tarball release.

Usage
=====

Windows
-------

TODO: We're still working on this. Stay tuned.

Unix
----

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

