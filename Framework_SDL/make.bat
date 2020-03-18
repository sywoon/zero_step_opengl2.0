@echo off

cls

:: 1 vs cl   2 clang
set USE_CL=1


if %USE_CL% == 1 (
    call clenv.bat
	cl *.c *.cpp /source-charset:utf-8 /D COMPILE_CL /D_CRT_SECURE_NO_WARNINGS /D _USING_V110_SDK71_ /I ".\include" /link /SUBSYSTEM:CONSOLE /LIBPATH:".\lib" /out:bin\main_cl.exe user32.lib gdi32.lib opengl32.lib glew32.lib SDL2.lib SDL2main.lib
	del *.obj
)

if %USE_CL% == 2 (
	clang -D _CRT_SECURE_NO_WARNINGS -I "include" -L "lib" -l user32.lib -l gdi32.lib -l opengl32.lib -l glew32.lib -l SDL2.lib -l SDL2main.lib  -o bin\main.exe *.cpp *.c -Xlinker /subsystem:console
)



pause
