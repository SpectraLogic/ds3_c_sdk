@echo off
set VISUAL_STUDIO_INSTALL_PATH=C:\Program Files (x86)\Microsoft Visual Studio 9.0
call "%VISUAL_STUDIO_INSTALL_PATH%\VC\vcvarsall.bat" x86
powershell -ExecutionPolicy RemoteSigned -File run_install_dot_bat_instead.ps1

