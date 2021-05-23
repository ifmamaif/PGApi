#pragma once

#include "PreDefines.h"

extern "C"
{

	EXPORT_API float** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY);

	EXPORT_API float& Get2DMapValue(float** buffer, int width, int height);

	EXPORT_API float OctavePerlin(float x, float y, float z, int octaves, float persistence);

	EXPORT_API float Fbm(float x, float y, float z, int numOctaves, float amplitude, float gain, float frequency, float lacunarity);
	EXPORT_API float Marbling(float x, float y, float z, float frequency, float amplitude);

}