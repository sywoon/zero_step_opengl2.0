@echo off

set "VSCOMM=C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7"
set "WINSDK=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A"
set "VSVC=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC"
set "WINKITS=C:\Program Files (x86)\Windows Kits\10\"
set "KITSVERSION=10.0.18362.0"

set "include=%VSVC%\INCLUDE;%VSVC%\ATLMFC\INCLUDE;%WINSDK%\INCLUDE;%WINKITS%\Include\%KITSVERSION%\ucrt;%include%;"
set "lib=%VSVC%\LIB;%VSVC%\ATLMFC\LIB;%WINSDK%\LIB;%WINKITS%\Lib\%KITSVERSION%\ucrt\x86;%lib%"
set "path=%VSVC%\BIN;%VSCOMM%\IDE;%VSCOMM%\TOOLS;%WINSDK%\BIN;%path%"

