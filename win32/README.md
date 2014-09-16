Overview
--------

This directory contains all of the files needed to build under Windows.
Currently we build using Microsoft's nmake and MSVC 9 and above.

Prerequesites
-------------

1. Powershell 4.0 or above 
2. .NET Framework 4.5 or above

Refer to "PowerShell DS3 Module" documentation in the DS3 Net sdk for information 
about PowerShell installation.


Basic Build
-----------

To create a release package, run `/win32/package.bat`. Note that this script
executes the Visual Studio 2008 Command Prompt environment script, so if you're
using a different version of Visual Studio you'll have to modify the Visual
Studio install path inside of `/win32/package.bat`.

To build the client including the sample executable do the following


1. Open a new Visual Studio Command Prompt.
2. Run the following command: `nmake -f Makefile.vc`
3. Makefile.vc places the resulting binaries inside of `/win32/output/bin` by
   default.

Dependencies
------------

The SDK depends upon several open source libraries. For convenience we
redistribute these dependencies in the `/win32/deps/install` directory.

To re-build the dependencies, follow the following procedure:

1. Delete the built dependency directory at `/win32/deps/install`.
2. Download and extract the dependency library packages.
   * The Unix shell script at `/win32/deps/build/download_and_unzip.sh` can do
     this for you, but you don't strictly need to use it. You can also manually
     download the URLs and extract using whatever utility you like. Note that
     the glib package must be extracted to `/win32/deps/build/prebuilt`.
3. The Windows batch file `/win32/deps/install.bat` compiles and installs the
   dependencies into the `/win32/deps/install` directory. It executes the
   Visual Studio 2008 Command Prompt environment script, so if you're using a
   different version you'll have to modify the Visual Studio install path inside
   of `install.bat`.

