#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API int Absi(int value);
	EXPORT_API float Absf(float value);
	EXPORT_API double Absd(double value);
	EXPORT_API int FastFloorf(float d);
	EXPORT_API int FastFloord(double d);

	EXPORT_API int FloorToIntf(float f);
	EXPORT_API int FloorToIntd(double f);

	EXPORT_API int RoundToInt(float f);

	// Function to linearly interpolate between a0 and a1
	// weight should be in the range [0.0, 1.0]
	EXPORT_API float Lerpf(float a0, float a1, float weight);

	// Function to linearly interpolate between a0 and a1
	// weight should be in the range [0.0, 1.0]
	EXPORT_API double Lerpd(double a0, double a1, double weight);

	EXPORT_API int Doti(int x, int y);
	EXPORT_API float Dotf(float x, float y);
	EXPORT_API float DotN(int nDim, const int g[], ...);
	EXPORT_API float DotArray(int nDim, const int g[], const float x[]);

	EXPORT_API int CheckBitStatus(int number, int bitPosition);
}