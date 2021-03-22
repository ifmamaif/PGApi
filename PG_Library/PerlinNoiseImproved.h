#pragma once

#include "PreDefines.h"

// Unity3D use PerlinNoise Improved

extern "C"
{
	EXPORT_API float PerlinNoise_Improved1D(float x);
	EXPORT_API float PerlinNoise_Improved2D(float x, float y);
	EXPORT_API float PerlinNoise_Improved3D(float x, float y, float z);
}