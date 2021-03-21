#pragma once

#include "PreDefines.h"

extern "C"
{

	EXPORT_API double SimplexNoise2D(double xin, double yin);
	EXPORT_API double SimplexNoise3D(double xin, double yin, double zin);
	EXPORT_API double SimplexNoise4D(double x, double y, double z, double w);

}