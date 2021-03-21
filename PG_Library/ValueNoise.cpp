#include "pch.h"
#include "ValueNoise.h"

#include "Constants.h"
#include "Allocation.h"
#include "Math.h"

float smoothstep(const float& t)
{
	return t * t * (3 - 2 * t);
}

float ValueNoise(const float x)
{
	int xInteger = FastFloor(x);

	float interpolation = x - xInteger;

	int xMin = xInteger % 256;
	int xMax = (xMin == 256 - 1) ? 0 : xMin + 1;

	return Lerpf((float)(g_HASH_TABLE_KEN_PERLIN[xMin]), (float)(g_HASH_TABLE_KEN_PERLIN[xMax]), interpolation);
}

float ValueNoise1D(const float x)
{
	const int xInteger = FastFloor(x);

	const float tx = x - xInteger;

	const int rx0 = xInteger & 255;
	const int rx1 = (rx0 + 1) & 255;

	// random values at the corners of the cell using permutation table
	const int& c00 = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[rx0]];
	const int& c10 = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[rx1]];

	// remapping of tx and ty using the Smoothstep function 
	const float sx = smoothstep(tx);

	// linearly interpolate values along the x axis
	return Lerpf((float)c00, (float)c10, sx);
}

float ValueNoise2D(const float x, const float y) 
{
	int xInteger = FastFloor(x);
	int yInteger = FastFloor(y);

	float tx = x - xInteger;
	float ty = y - yInteger;

	int rx0 = xInteger & 255;
	int rx1 = (rx0 + 1) & 255;
	int ry0 = yInteger & 255;
	int ry1 = (ry0 + 1) & 255;

	// random values at the corners of the cell using permutation table
	const int& c00 = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[rx0] + ry0]];
	const int& c10 = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[rx1] + ry0]];
	const int& c01 = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[rx0] + ry1]];
	const int& c11 = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[rx1] + ry1]];

	// remapping of tx and ty using the Smoothstep function 
	float sx = smoothstep(tx);
	float sy = smoothstep(ty);

	// linearly interpolate values along the x axis
	float nx0 = Lerpf((float)c00, (float)c10, sx);
	float nx1 = Lerpf((float)c01, (float)c11, sx);

	// linearly interpolate the nx0/nx1 along they y axis
	return Lerpf(nx0, nx1, sy);
}

float ValueNoiseND(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0.0;
	}

	const int PERMUTATIONS = 2 << (nDim - 1);
	int interpolations = nDim;

	float* input = new float[nDim];
	float* inputFloats = new float[nDim];
	float* corners = new float[PERMUTATIONS];

	VA_LIST_GET(nDim, float, input);

	int* inputInteger = new int[nDim];
	for (int i = 0; i < nDim; i++)
	{
		inputInteger[i] = FastFloor(input[i]);
		inputFloats[i] = input[i] - inputInteger[i];
		inputInteger[i] = inputInteger[i] & 255;
	}
	//inputInteger[0] = FastFloor(input[0]);
	//inputFloats[0] = input[0] - inputInteger[0];
	//inputInteger[0] = inputInteger[0] & 255;

	// random values at the corners of the cell using permutation table	
	for (int i = 0; i < PERMUTATIONS; i++)
	{
		corners[i] = 0;
		for (int j = 0; j < nDim; j++)
		{
			corners[i] += g_HASH_TABLE_KEN_PERLIN[inputInteger[j] + CheckBitStatus(i, nDim - 1 - j) + (int)(corners[i])];
			int bit = CheckBitStatus(i, nDim - 1 - j);
			//corners[i] = (float)g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[inputInteger[j] + bit]];
		}
		corners[i] = (float)g_HASH_TABLE_KEN_PERLIN[(int)(corners[i])];
	}
	//corners[0] = (float)g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[inputInteger[0]]];
	//corners[1] = (float)g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[inputInteger[0] + 1]];


	// remapping of tx and ty using the Smoothstep function 
	for (int i = 0; i < nDim; i++)
	{
		inputFloats[i] = smoothstep(inputFloats[i]);
	}
	//inputFloats[0] = smoothstep(inputFloats[0]);

	// linearly interpolate values along the x,y,... axis
	for (int i = 0; i < nDim; i++)
	{
		for (int j = 0; j < interpolations; j++)
		{
			corners[j] = Lerpf((float)corners[j], (float)corners[j + interpolations], inputFloats[i]);
		}
		interpolations /= 2;
	}
	//corners[0] = Lerpf((float)corners[0], (float)corners[1], inputFloats[0]);
	
	float result = corners[0];

	delete[] inputInteger;
	delete[] inputFloats;
	delete[] input;

	return result;
}