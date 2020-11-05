// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "PreDefines.h"

#include "MazeWorm.h"

extern "C"
{
    EXPORT_API int DADA() { return 13; }

    EXPORT_API const char* PrintHello() {
        return "Hello";
    }

}
