#pragma once

#ifdef DEBUG
    
    #include <windows.h>
    #include <stdarg.h>
    #include <stdio.h>

    void debugPrint(const char *format, ...);

    // #ifdef SKB_DEBUG

    void debugPrint(const char *format, ...){
        // #if defined(DEBUG)
            char buffer[1024];

            va_list args;
            va_start(args, format);

            vsnprintf(buffer, sizeof(buffer), format, args);

            va_end(args);

            OutputDebugStringA(buffer);
        // #endif
    }


#endif