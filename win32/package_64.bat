@echo off

set VISUAL_STUDIO_INSTALL_PATH=C:\Program Files (x86)\Microsoft Visual Studio 14.0

call "%VISUAL_STUDIO_INSTALL_PATH%\VC\vcvarsall.bat" x86_amd64

powershell -ExecutionPolicy RemoteSigned -File run_package_dot_bat_instead_64.ps1

