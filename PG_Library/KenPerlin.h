#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API double SimplexNoise2D(double xin, double yin);
	EXPORT_API double SimplexNoise3D(double xin, double yin, double zin);
	EXPORT_API double SimplexNoise4D(double x, double y, double z, double w);

	EXPORT_API double PerlinNoiseUnity(double x);
	EXPORT_API double PerlinNoise2DUnity(double x, double y);
	EXPORT_API double PerlinNoise3DUnity(double x, double y, double z);

	EXPORT_API double** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY);

	EXPORT_API double& Get2DMapValue(double** buffer, int width, int height);

	EXPORT_API double ClassicPerlinNoise1D(double x);
	EXPORT_API double ClassicPerlinNoise2D(double x, double y);
	EXPORT_API double ClassicPerlinNoise3D(double x, double y, double z);

	EXPORT_API double PerlinNoiseND(int nDim, ...);
	EXPORT_API double PerlinNoiseNDArray(int nDim, double* input);


	EXPORT_API double* ClassicPerlinNoise1D_Test_Input(double x);
	EXPORT_API double* PerlinNoiseND_Test_Input(int nDim, ...);
	EXPORT_API int* ClassicPerlinNoise1D_Test_UnitGrid(double x);
	EXPORT_API int* PerlinNoiseND_Test_UnitGrid(int nDim, ...);
	EXPORT_API double* ClassicPerlinNoise1D_Test_Relative(double x);
	EXPORT_API double* PerlinNoiseND_Test_Relative(int nDim, ...);
	EXPORT_API int* ClassicPerlinNoise1D_Test_Wrap(double x);
	EXPORT_API int* PerlinNoiseND_Test_Wrap(int nDim, ...);
	EXPORT_API int* ClassicPerlinNoise1D_Test_Gradients(double x);
	EXPORT_API int* PerlinNoiseNDArray_Test_Gradients(int nDim, ...);
}