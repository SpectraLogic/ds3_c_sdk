# download 64bit library dependencies and put into expected layout for install.

. "$PSScriptRoot\util.ps1" # provides Unzip and Untar

(new-object System.Net.WebClient).DownloadFile('http://curl.haxx.se/download/curl-7.37.0.zip','curl-7.37.0.zip')
Unzip curl-7.37.0.zip .
Remove-Item curl-7.37.0.zip

(new-object System.Net.WebClient).DownloadFile('http://zlib.net/zlib1211.zip','zlib1211.zip')
Unzip zlib1211.zip .
Remove-Item zlib1211.zip

(new-object System.Net.WebClient).DownloadFile('ftp://xmlsoft.org/libxml2/libxml2-2.9.4.tar.gz','libxml2-2.9.4.tar.gz')
Untar libxml2-2.9.4.tar.gz .
Remove-Item libxml2-2.9.4.tar.gz

(new-object System.Net.WebClient).DownloadFile('http://win32builder.gnome.org/gtk+-bundle_3.6.4-20131201_win64.zip','gtk+-bundle_3.6.4-20131201_win64.zip')
Unzip gtk+-bundle_3.6.4-20131201_win64.zip prebuilt
Remove-Item gtk+-bundle_3.6.4-20131201_win64.zip
