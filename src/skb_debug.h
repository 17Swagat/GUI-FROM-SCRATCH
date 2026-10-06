#pragma once

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

void debugPrint(const char *format, ...);

#ifdef DEBUG

void debugPrint(const char *format, ...){
    char buffer[1024];

    va_list args;
    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    OutputDebugStringA(buffer);
}



#endif