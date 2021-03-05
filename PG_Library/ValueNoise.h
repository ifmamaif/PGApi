#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float ValueNoise(const float x);
	EXPORT_API float ValueNoise1D(const float x);
	EXPORT_API float ValueNoise2D(const float x,const float y);
	EXPORT_API float ValueNoiseND(const int nDim,...);
}
