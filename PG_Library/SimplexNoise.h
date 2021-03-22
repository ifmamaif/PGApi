#pragma once

#include "PreDefines.h"

extern "C"
{

	EXPORT_API float SimplexNoise2D(float xin, float yin);
	EXPORT_API float SimplexNoise3D(float xin, float yin, float zin);
	EXPORT_API float SimplexNoise4D(float x, float y, float z, float w);

}