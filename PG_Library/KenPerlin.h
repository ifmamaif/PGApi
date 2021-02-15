#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API double SimplexNoise2D(double xin, double yin);
	EXPORT_API double SimplexNoise3D(double xin, double yin, double zin);
	EXPORT_API double SimplexNoise4D(double x, double y, double z, double w);

	EXPORT_API double PerlinNoise(double x);
	EXPORT_API double PerlinNoise2D(double x, double y);
	EXPORT_API double PerlinNoise3D(double x, double y, double z);

	EXPORT_API double** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY);

	EXPORT_API double& Get2DMapValue(double** buffer, int width, int height);

	EXPORT_API double* ClassicPerlinNoise3D(double x, double y, double z);

	EXPORT_API double* PerlinNoiseND(int nDim, ...);
}