#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float SimplexNoise2D(float x, float y);
	EXPORT_API float SimplexNoise3D(float x, float y, float z);
	EXPORT_API float SimplexNoise4D(float x, float y, float z, float w);
}