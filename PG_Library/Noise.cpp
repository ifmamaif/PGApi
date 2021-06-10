#include "pch.h"
#include "Noise.h"

#include "KenPerlin.h"
#include "Math.h"
#include <math.h>
#include "PerlinNoiseImproved.h"

float** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY)
{
	float** noiseMap = new float* [width];
	for (int i = 0; i < width; i++)
	{
		noiseMap[i] = new float[height];
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float xCoord = (float)x / width * scale + offsetX;
			float yCoord = (float)y / height * scale + offsetY;
			noiseMap[x][y] = (float)PerlinNoise_Improved3D(xCoord, yCoord, 0.f);
		}
	}
	return noiseMap;
}

float& Get2DMapValue(float** buffer, int width, int height)
{
	return buffer[width][height];
}

float OctavePerlin(float x, float y, float z, int octaves, float persistence)
{
	float total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;                            // Used for normalizing result to 0.0 - 1.0
	for (int i = 0; i < octaves; i++)
	{
		total += PerlinNoise_Improved3D(x * frequency, y * frequency, z * frequency) * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		frequency *= 2;
	}

	return total / maxValue;
}

//Fractional Brownian motion (Turbulence function)
float Fbm(float x, float y, float z, int numOctaves, float amplitude, float gain, float frequency, float lacunarity)
{
	// Gain (amplitude multiplier) controls amplitude change between each band. Typically: gain = 0.5
	// Lacunarity (frequency multiplier) controls frequency change between each band. Typically: lacunarity = 2

	// octaves = the number of layers
	// frequency = magnification

	// Noise with many frequencies present looks more natural 
	// can get this by summing noise at different magnifications
	float noiseSum = 0;
	for (int i = 0; i < numOctaves; i++)
	{
		noiseSum += amplitude * PerlinNoise_Improved3D(x*frequency, y * frequency, z * frequency);
		amplitude *= gain;
		frequency *= lacunarity;
	}
	return noiseSum;
}

float Marbling(float x, float y, float z, float frequency, float amplitude)
{
	return sin(frequency * (x + amplitude * Fbm(x, y, z,8, amplitude,.5f, frequency,2)));
}
