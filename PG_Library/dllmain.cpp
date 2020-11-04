// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this

extern "C"
{
    EXPORT_API int DADA() { return 13; }

    EXPORT_API const char* PrintHello() {
        return "Hello";
    }

}
