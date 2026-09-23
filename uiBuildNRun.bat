@echo off

if not exist build mkdir build

pushd build
@REM  (cl /nologo /Zi /Od /EHsc /std:c++17 ..\src\win32_platform.cpp /link /INCREMENTAL:NO) && (win32_platform.exe)
(cl /nologo /Zi /Od /EHsc /std:c++20 ..\src\win32_platform.cpp /link /INCREMENTAL:NO) && (win32_platform.exe)
popd

@REM /EHsc is used for what? It is used to specify the exception handling model for C++ code. The /EHsc option tells the compiler to enable standard C++ exception handling, which means that it will generate code that can handle exceptions thrown by C++ code and will also ensure that destructors are called for objects that go out of scope when an exception is thrown. This option is important for writing robust C++ code that can handle errors and exceptions gracefully.