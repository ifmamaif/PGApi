#pragma once

#include "PreDefines.h"

double Noise(double x, double y, double z);

double Noise(double x, double y);

double Noise(double x);

extern "C"
{
	EXPORT_API double** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY);
}