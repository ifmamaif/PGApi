#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float** GenereazăHartăPerlin2D(int width, int height, float scale, float offsetX, float offsetY);

	EXPORT_API float& IaValoareDinHartaPerlin2D(float** buffer, int width, int height);

	EXPORT_API float OctavăPerlin(float x, float y, float z, int octaves, float persistence);

	EXPORT_API float MișcareFracționatăBrowniană(float x, float y, float z, int numOctaves, float amplitude, float gain, float frequency, float lacunarity);
	EXPORT_API float ImitaţieMarmoră(float x, float y, float z, float frequency, float amplitude);
}