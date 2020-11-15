// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "PreDefines.h"

#include "KenPerlin.h"
#include "Math.h"
#include "MazeWorm.h"
#include "Random.h"
#include "Utils.h"

extern "C"
{
    EXPORT_API int DADA() { return 13; }

    EXPORT_API int* DADA222()
    { 
        int* buffer = new int[2];
        buffer[0] = 1;
        buffer[1] = 2;
        return buffer;
    }

    EXPORT_API int** DADA22233()
    {
        int** buffer = new int*[2];

        buffer[0][0] = 1;
        buffer[0][1] = 2;
        return buffer;
    }

}
