#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float ClassicPerlinNoise1D(float x);
	EXPORT_API float ClassicPerlinNoise2D(float x, float y);
	EXPORT_API float ClassicPerlinNoise3D(float x, float y, float z);

	EXPORT_API float PerlinNoiseND(int nDim, ...);
	EXPORT_API float PerlinNoiseNDArray(int nDim, float* input);

	EXPORT_API float* ClassicPerlinNoise1D_Test_Input(float x);
	EXPORT_API float* PerlinNoiseND_Test_Input(int nDim, ...);
	EXPORT_API int* ClassicPerlinNoise1D_Test_UnitGrid(float x);
	EXPORT_API int* PerlinNoiseND_Test_UnitGrid(int nDim, ...);
	EXPORT_API float* ClassicPerlinNoise1D_Test_Relative(float x);
	EXPORT_API float* PerlinNoiseND_Test_Relative(int nDim, ...);
	EXPORT_API int* ClassicPerlinNoise1D_Test_Wrap(float x);
	EXPORT_API int* PerlinNoiseND_Test_Wrap(int nDim, ...);
	EXPORT_API int* ClassicPerlinNoise1D_Test_Gradients(float x);
	EXPORT_API int* PerlinNoiseNDArray_Test_Gradients(int nDim, ...);
}