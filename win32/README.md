Overview
--------

This directory contains all of the files needed to build under Windows.
Currently we build using CMake and MSVC 2015 and above.

Prerequisites
-------------

1. Boost C++ library installed to a default location on the machine
2. PowerShell 3+
3. If you don't already have 7zip, it will be downloaded and placed in the Windows Temp directory to untar glib.

Dependencies
------------

The SDK depends upon several open source libraries.

To acquire the dependencies, follow the following procedure:

1. Ensure the built dependency directory at `/win32/deps/install` is empty.
2. Download and extract the dependency library packages.  Depending on whether you want 32 or 64 bit libs, run one of the following:
   * `\win32\deps\build\download_and_unzip.bat`
   * `\win32\deps\build\download_and_unzip_64.bat`
   * You can also manually download the URLs and extract using whatever utility you like. Note that the glib package must be extracted to `/win32/deps/build/prebuilt`.
3. The Windows batch file `/win32/deps/install(_64).bat` compiles and installs the dependencies into the `/win32/deps/install` directory. It executes the Visual Studio 14 2015 Command Prompt environment script, so if you're using a different version you'll have to modify the Visual Studio install path inside of `install(_64).bat`.


Build
-----------

To build the client library


1. Open a new Visual Studio Command Prompt.
2. Navigate to the ds3_c_sdk\win32 directory.
3. Run `package(_64).bat`, which will use CMake to create a Visual Studio 14 2015 solution, and then compile from the command line using msbuild, and compress all requiredSDK dependencies into a `ds3_win(32 or 64).zip`
4. This should build the libds3 library, which is placed in the directory '<root of repo>/win32/output/bin'


Unit Tests
----------

1. Open a new Visual Studio Command Prompt.
2. Navigate to the ds3_c_sdk/test directory.
3. Run the following command: `cmake ..\.. -G "Visual Studio 14 2015 {Win64}"`
4. Either open the resulting 'libds3_tests.sln' in Visual Studio and build it
  from there, or run the command 'msbuild libds3_tests.sln' from the command
  prompt. This should build the libds3 library, which is placed in the directory
  '<root of repo>/tests/bin'
  

