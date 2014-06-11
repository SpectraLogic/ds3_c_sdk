$depsFolder = pwd
$installFolder = "$depsFolder\install\"

Function Install($sourceFolder, $file)
{
    $source = $sourceFolder + $file
    $destination = $installFolder + $file
    $destinationFolder = Split-Path $destination -Parent
    mkdir -Force $destinationFolder > $null
    copy $source $destination -Recurse -Force > $null
}

Function Install-Prebuilt($file)
{
    Install "build\prebuilt\" $file
}

# Install the glib.
Install-Prebuilt bin\libglib-2.0-0.dll
Install-Prebuilt bin\libintl-8.dll
Install-Prebuilt bin\libiconv-2.dll
Install-Prebuilt bin\pthreadGC2.dll
Install-Prebuilt lib\glib-2.0
Install-Prebuilt lib\glib-2.0.lib
Install-Prebuilt include\glib-2.0

# Compile and install zlib.
$zlib = "build\zlib-1.2.8"
cd $zlib
nmake -f win32\Makefile.msc
cd ..\..
copy "$zlib\zlib.h" "$installFolder\include"
copy "$zlib\zconf.h" "$installFolder\include"
copy "$zlib\zlib.lib" "$installFolder\lib\zlib_a.lib"

# Compile and install libcurl.
$curl = "build\curl-7.37.0"
cd "$curl\winbuild"
nmake -f Makefile.vc mode=static WITH_DEVEL=$installFolder WITH_ZLIB=static ENABLE_IDN=no
cd ..\..\..
copy "$curl\builds\libcurl-vc-x86-release-static-zlib-static-ipv6-sspi-spnego-winssl\lib\libcurl_a.lib" "$installFolder\lib"
xcopy /s "$curl\builds\libcurl-vc-x86-release-static-zlib-static-ipv6-sspi-spnego-winssl\include" "$installFolder\include"

# Compile and install libxml.
$libxml2 = "build\libxml2-2.9.1"
cd "$libxml2\win32"
cscript configure.js include="$installFolder\include" lib="$installFolder\lib" iconv=no
nmake -f Makefile.msvc
cd ..\..\..
copy "$libxml2\win32\bin.msvc\libxml2_a.lib" "$installFolder\lib"
mkdir "$installFolder\include\libxml"
copy "$libxml2\include\libxml\*.h" "$installFolder\include\libxml"

