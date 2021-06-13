#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float PerlinNoise_Classic1D(float x);
	EXPORT_API float PerlinNoise_Classic2D(float x, float y);
	EXPORT_API float PerlinNoise_Classic3D(float x, float y, float z);

	EXPORT_API float PerlinNoiseND(int nDim, ...);
	EXPORT_API float PerlinNoiseNDArray(int nDim, float* input);
}