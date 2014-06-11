#!/bin/sh

wget http://curl.haxx.se/download/curl-7.37.0.zip
wget http://zlib.net/zlib128.zip
wget ftp://xmlsoft.org/libxml2/libxml2-2.9.1.tar.gz
wget http://win32builder.gnome.org/gtk%2B-bundle_3.6.4-20130921_win32.zip

unzip curl-7.37.0.zip
unzip zlib128.zip
tar zxf libxml2-2.9.1.tar.gz
unzip -d prebuilt gtk+-bundle_3.6.4-20130921_win32.zip

