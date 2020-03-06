@echo off

cls
call clenv.bat
call winkitenv.bat

:: 1 vs cl   2 clang
set USE_CL=1


cd src

if %USE_CL% == 1 (
	cl *.cpp /source-charset:utf-8 /D COMPILE_CL /D_CRT_SECURE_NO_WARNINGS /D _USING_V110_SDK71_ /I "..\..\third_party\glew" /link /LIBPATH:"..\..\third_party\libraries" /out:..\main_cl.exe user32.lib gdi32.lib opengl32.lib glew32.lib
	del *.obj
)

if %USE_CL% == 2 (
	clang -D _CRT_SECURE_NO_WARNINGS -I "..\..\third_party\glew" -L "..\..\third_party\libraries" -l user32.lib -l gdi32.lib -l opengl32.lib -l glew32.lib  -o ..\main.exe *.cpp
)



pause
