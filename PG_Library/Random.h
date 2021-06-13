#pragma once

#include "PreDefines.h"
#include <time.h>

extern "C"
{
    EXPORT_API const struct tm* GetCurrentLocalTime();
    EXPORT_API int Rand();
    EXPORT_API int Rand_s();
    EXPORT_API int RandNewSeed(int newseed);
    EXPORT_API int RandBetween(int minInclusive, int maxExclusive);
    // Random value between 0 and 1
    EXPORT_API float Randf();
}