@echo off

@set "LLVMPath=C:\Program Files\LLVM"
@set "VSPath=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community"
@set "VSVersion=14.20.27404"
@set "WinSDKVersion=10.0.17763.0"
@set "WinSDKPath=C:\Program Files (x86)\Windows Kits\10"
@set "VSBasePath=%VSPath%\VC\Tools\MSVC\%VSVersion%"
@set "PATH=%PATH%;%LLVMPath%\bin;%VSBasePath%\bin\HostX64\x64"

@set CompilerFlags= ^
	-std=c++17 ^
	-stdlib=libc++ ^
    -Wno-everything

@set LinkFlags= ^
	-machine:x64 ^
	-subsystem:console

@set LinkLibs= ^
	/libpath:"%VSBasePath%\lib\x64" ^
	/libpath:"%WinSDKPath%\Lib\%WinSDKVersion%\ucrt\x64" ^
	/libpath:"%WinSDKPath%\Lib\%WinSDKVersion%\um\x64" ^
	/libpath:"C:\Program Files\LLVM\lib" ^
	libcmt.lib User32.lib pdfgen.lib


@echo Compiling...
::@clang -shared -g -gcodeview pdfgen.c -o pdfgen.lib -c -Wno-everything
@clang -g -gcodeview bmo.cc -o bmo.o -c %CompilerFlags%
@lld-link.exe  /debug "bmo.o" /OUT:"bmo.exe" %LinkFlags% %LinkLibs% 

del *.o
