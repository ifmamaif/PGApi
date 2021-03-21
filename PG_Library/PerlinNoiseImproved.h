#pragma once

#include "PreDefines.h"

// Unity3D use PerlinNoise Improved

extern "C"
{
	EXPORT_API double PerlinNoise_Improved1D(double x);
	EXPORT_API double PerlinNoise_Improved2D(double x, double y);
	EXPORT_API double PerlinNoise_Improved3D(double x, double y, double z);
}