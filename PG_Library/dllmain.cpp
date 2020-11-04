// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this

extern "C"
{
    EXPORT_API int DADA() { return 13; }

    // The functions we will call from Unity.
    //
    EXPORT_API const char* PrintHello() {
        return "Hello";
    }

}

//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//                     )
//{
//    switch (ul_reason_for_call)
//    {
//    case DLL_PROCESS_ATTACH:
//    case DLL_THREAD_ATTACH:
//    case DLL_THREAD_DETACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//    return TRUE;
//}
