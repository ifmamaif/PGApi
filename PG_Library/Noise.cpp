#include "pch.h"
#include "Noise.h"

#include "KenPerlin.h"
#include "Math.h"
#include <math.h>
#include "PerlinNoiseImproved.h"

double** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY)
{
	//double** noiseMap = (double**)malloc(sizeof(double*)*width);
	//for (int i = 0; i < width; i++)
	//{
	//	noiseMap[i] = (double*)malloc(sizeof(double)*height);
	//}
	double** noiseMap = new double* [width];
	for (int i = 0; i < width; i++)
	{
		noiseMap[i] = new double[height];
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			double xCoord = (double)x / width * scale + offsetX;
			double yCoord = (double)y / height * scale + offsetY;
			noiseMap[x][y] = (double)PerlinNoise_Improved3D(xCoord, yCoord, 0.f);
		}
	}
	return noiseMap;
}

double& Get2DMapValue(double** buffer, int width, int height)
{
	return buffer[width][height];
}

double OctavePerlin(double x, double y, double z, int octaves, double persistence)
{
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	double maxValue = 0;                            // Used for normalizing result to 0.0 - 1.0
	for (int i = 0; i < octaves; i++)
	{
		total += PerlinNoise_Improved3D(x * frequency, y * frequency, z * frequency) * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		frequency *= 2;
	}

	return total / maxValue;
}

double Turbulence(double x, double y, double z, int octaves, double amplitude, double magnification)
{
	// Noise with many frequencies present looks more natural 
	// can get this by summing noise at different magnifications
	double result = 0;
	for (int i = 0; i < octaves; i++)
	{
		result += amplitude * PerlinNoise_Improved3D(x, y, z);

		magnification *= 2;
		amplitude /= 2;
	}
	return result;
}

double Marbling(double x, double y, double z, double frequency, double amplitude)
{
	// f = marble pattern frequency
	// a = marble noise amplitude

	//marble(x,y,z) = sin(f*(x+a*turb(x,y,z)))
	return sin(frequency * (x + amplitude * Turbulence(x, y, z,8, amplitude, frequency)));
}
