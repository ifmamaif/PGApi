#pragma once

#include "PreDefines.h"
#include <time.h>

extern "C"
{
    EXPORT_API const struct tm* ObțineOraCurentă();
    EXPORT_API int AleatoriuIntreg();
    EXPORT_API int Aleatoriu_Securizat();
    EXPORT_API int AleatoriuNouăSămânța(int newseed);
    EXPORT_API int AleatoriuÎntre(int minInclusive, int maxExclusive);
    EXPORT_API float Aleatoriu01f();
}