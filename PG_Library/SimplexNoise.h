#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float ZgomotulPerlin_Simplex1D(float x);
	EXPORT_API float ZgomotulPerlin_Simplex2D(float x, float y);
	EXPORT_API float ZgomotulPerlin_Simplex3D(float x, float y, float z);
	EXPORT_API float ZgomotulPerlin_Simplex4D(float x, float y, float z, float w);
}