Overview
--------

This directory contains all of the files needed to build under Windows.
Currently we build using cmake and MSVC 2015 and above.

Prerequisites
-------------

1. Boost C++ library installed to a default location on the machine


Basic Build
-----------

To build the client library


1. Open a new Visual Studio Command Prompt.
2. Navigate to the ds3_c_sdk directory.
3. Run the following command: `cmake -G "Visual Studio 14 2015"`
4. Either open the resulting 'libds3.sln' in Visual Studio and build it from
  there, or run the command 'msbuild libds3.sln' from the command prompt.
  This should build the libds3 library, which is placed in the directory '<root of repo>/win32/output/bin'


Unit Tests
----------

1. Open a new Visual Studio Command Prompt.
2. Navigate to the ds3_c_sdk/test directory.
3. Run the following command: `cmake -G "Visual Studio 14 2015"`
4. Either open the resulting 'libds3_tests.sln' in Visual Studio and build it
  from there, or run the command 'msbuild libds3_tests.sln' from the command
  prompt. This should build the libds3 library, which is placed in the directory
  '<root of repo>/tests/bin'
  

Dependencies
------------

The SDK depends upon several open source libraries. For convenience we
redistribute these dependencies in the `/win32/deps/install` directory,
except for the Boost library headers.

To re-build the dependencies, follow the following procedure:

1. Delete the built dependency directory at `/win32/deps/install`.
2. Download and extract the dependency library packages.
   * The Unix shell script at `/win32/deps/build/download_and_unzip.sh` can do
     this for you, but you don't strictly need to use it. You can also manually
     download the URLs and extract using whatever utility you like. Note that
     the glib package must be extracted to `/win32/deps/build/prebuilt`.
3. The Windows batch file `/win32/deps/install.bat` compiles and installs the
   dependencies into the `/win32/deps/install` directory. It executes the
   Visual Studio 2012 Command Prompt environment script, so if you're using a
   different version you'll have to modify the Visual Studio install path inside
   of `install.bat`.

