#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float SimplexNoise2D(float x, float y);
	EXPORT_API float SimplexNoise3D(float x, float y, float z);
	EXPORT_API float SimplexNoise4D(float x, float y, float z, float w);

	EXPORT_API float noise_simplex(float xin, float yin);
	EXPORT_API float noise_simplex_optimized(float xin, float yin);
}