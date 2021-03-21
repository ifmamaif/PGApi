#pragma once

#include "PreDefines.h"

extern "C"
{

	EXPORT_API double** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY);

	EXPORT_API double& Get2DMapValue(double** buffer, int width, int height);

	EXPORT_API double OctavePerlin(double x, double y, double z, int octaves, double persistence);

	EXPORT_API double Turbulence(double x, double y, double z, int octaves, double amplitude,double magnification);
}