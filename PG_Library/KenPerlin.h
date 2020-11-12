#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API double PerlinNoise3D(double x, double y, double z);
	
	EXPORT_API double PerlinNoise2D(double x, double y);
	
	EXPORT_API double PerlinNoise(double x);

	EXPORT_API double** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY);
}