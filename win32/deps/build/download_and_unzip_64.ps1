# download 64bit library dependencies and put into expected layout for install.

. "$PSScriptRoot\util.ps1" # provides Unzip and Untar

(new-object System.Net.WebClient).DownloadFile('http://curl.haxx.se/download/curl-7.37.0.zip','curl-7.37.0.zip')
unzip curl-7.37.0.zip

(new-object System.Net.WebClient).DownloadFile('http://zlib.net/zlib128.zip','zlib128.zip')
unzip zlib128.zip

(new-object System.Net.WebClient).DownloadFile('ftp://xmlsoft.org/libxml2/libxml2-2.9.4.tar.gz','libxml2-2.9.4.tar.gz')
Untar libxml2-2.9.4.tar.gz libxml2-2.9.4
del libxml2-2.9.4.tar

(new-object System.Net.WebClient).DownloadFile('http://win32builder.gnome.org/gtk+-bundle_3.6.4-20131201_win64.zip','gtk+-bundle_3.6.4-20131201_win64.zip')
unzip gtk+-bundle_3.6.4-20131201_win64.zip prebuilt
del gtk+-bundle_3.6.4-20131201_win64.zip
