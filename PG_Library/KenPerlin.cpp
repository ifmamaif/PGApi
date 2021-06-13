#include "pch.h"
#include "KenPerlin.h"

#include "Math.h"
#include "Constants.h"
#include "Allocation.h"
#include "PerlinUtils.h"

float PerlinNoise_Classic1D(float x)
{
	// Find unit grid cell containing point
	int xi = FastFloorf(x);

	// Get relative x coordinates of point within that cell
	x = x - xi;

	// Wrap the integer cells at 255 (smaller integer period can be introduced here)
	xi = xi & 255;

	// Calculate a set of eight hashed gradient indices
	int gi0 = g_HASH_TABLE_KEN_PERLIN[xi] % 12;
	int gi1 = g_HASH_TABLE_KEN_PERLIN[xi + 1] % 12;

	// Calculate noise contributions from each of the eight corners (2 corners in 1 dimension)
	float n0 = Dotf((float)g_GRAD3[gi0][0], x);
	float n1 = Dotf((float)g_GRAD3[gi1][0], x - 1);

	// Compute the fade curve value of x
	float fc = HermiteBlendingFunctionf(x);

	// Interpolate along x the contributions from each of the corners
	float nx = Lerpf(n0, n1, fc);

	return nx;
}

float PerlinNoise_Classic2D(float x, float y)
{
	// Find unit grid cell containing point
	int xi = FastFloorf(x);
	int yi = FastFloorf(y);

	// Get relative xyz coordinates of point within that cell
	x = x - xi;
	y = y - yi;

	// Wrap the integer cells at 255 (smaller integer period can be introduced here)
	xi = xi & 255;
	yi = yi & 255;

	// Calculate a set of eight hashed gradient indices
	int gi00 = g_HASH_TABLE_KEN_PERLIN[xi + g_HASH_TABLE_KEN_PERLIN[yi]] % 12;
	int gi01 = g_HASH_TABLE_KEN_PERLIN[xi + g_HASH_TABLE_KEN_PERLIN[yi + 1]] % 12;
	int gi10 = g_HASH_TABLE_KEN_PERLIN[xi + 1 + g_HASH_TABLE_KEN_PERLIN[yi]] % 12;
	int gi11 = g_HASH_TABLE_KEN_PERLIN[xi + 1 + g_HASH_TABLE_KEN_PERLIN[yi+1]] % 12;

	float xMin = x - 1;
	float yMin = y - 1;

	// Calculate noise contributions from each of the eight corners
	float n00 = DotN(2,g_GRAD3[gi00], x, y);
	float n01 = DotN(2,g_GRAD3[gi01], x, yMin);
	float n10 = DotN(2,g_GRAD3[gi10], xMin, y);
	float n11 = DotN(2,g_GRAD3[gi11], xMin, yMin);

	// Compute the fade curve value for each of x, y, z
	float u = HermiteBlendingFunctionf(x);
	float v = HermiteBlendingFunctionf(y);

	// Interpolate along x the contributions from each of the corners
	float nx0 = Lerpf(n00, n10, u);
	float nx1 = Lerpf(n01, n11, u);

	// Interpolate the four results along y
	float nxy = Lerpf(nx0, nx1, v);

	return nxy;
}

float FadeOriginal(float x)
{
	return (3 - 2 * x) * x * x;
}

float PerlinNoise_Classic3D(float x, float y, float z)
{
	// Find unit grid cell containing point
	int xi = FastFloorf(x);
	int yi = FastFloorf(y);
	int zi = FastFloorf(z);

	// Get relative xyz coordinates of point within that cell
	x = x - xi;
	y = y - yi;
	z = z - zi;

	// Wrap the integer cells at 255 (smaller integer period can be introduced here)
	xi = xi & 255;
	yi = yi & 255;
	zi = zi & 255;

	// Calculate a set of eight hashed gradient indices
	int gi000 = g_HASH_TABLE_KEN_PERLIN[xi + g_HASH_TABLE_KEN_PERLIN[yi + g_HASH_TABLE_KEN_PERLIN[zi]]] % 12;
	int gi001 = g_HASH_TABLE_KEN_PERLIN[xi + g_HASH_TABLE_KEN_PERLIN[yi + g_HASH_TABLE_KEN_PERLIN[zi + 1]]] % 12;
	int gi010 = g_HASH_TABLE_KEN_PERLIN[xi + g_HASH_TABLE_KEN_PERLIN[yi + 1 + g_HASH_TABLE_KEN_PERLIN[zi]]] % 12;
	int gi011 = g_HASH_TABLE_KEN_PERLIN[xi + g_HASH_TABLE_KEN_PERLIN[yi + 1 + g_HASH_TABLE_KEN_PERLIN[zi + 1]]] % 12;
	int gi100 = g_HASH_TABLE_KEN_PERLIN[xi + 1 + g_HASH_TABLE_KEN_PERLIN[yi + g_HASH_TABLE_KEN_PERLIN[zi]]] % 12;
	int gi101 = g_HASH_TABLE_KEN_PERLIN[xi + 1 + g_HASH_TABLE_KEN_PERLIN[yi + g_HASH_TABLE_KEN_PERLIN[zi + 1]]] % 12;
	int gi110 = g_HASH_TABLE_KEN_PERLIN[xi + 1 + g_HASH_TABLE_KEN_PERLIN[yi + 1 + g_HASH_TABLE_KEN_PERLIN[zi]]] % 12;
	int gi111 = g_HASH_TABLE_KEN_PERLIN[xi + 1 + g_HASH_TABLE_KEN_PERLIN[yi + 1 + g_HASH_TABLE_KEN_PERLIN[zi + 1]]] % 12;

	float xMin = x - 1;
	float yMin = y - 1;
	float zMin = z - 1;

	// Calculate noise contributions from each of the eight corners
	float n000 = DotN(3, g_GRAD3[gi000], x, y, z);
	float n001 = DotN(3, g_GRAD3[gi001], x, y, zMin);
	float n010 = DotN(3, g_GRAD3[gi010], x, yMin, z);
	float n011 = DotN(3, g_GRAD3[gi011], x, yMin, zMin);
	float n100 = DotN(3, g_GRAD3[gi100], xMin, y, z);
	float n101 = DotN(3, g_GRAD3[gi101], xMin, y, zMin);
	float n110 = DotN(3, g_GRAD3[gi110], xMin, yMin, z);
	float n111 = DotN(3, g_GRAD3[gi111], xMin, yMin, zMin);

	// Compute the fade curve value for each of x, y, z
	float u = HermiteBlendingFunctionf(x);
	float v = HermiteBlendingFunctionf(y);
	float w = HermiteBlendingFunctionf(z);

	// Interpolate along x the contributions from each of the corners
	float nx00 = Lerpf(n000, n100, u);
	float nx01 = Lerpf(n001, n101, u);
	float nx10 = Lerpf(n010, n110, u);
	float nx11 = Lerpf(n011, n111, u);

	// Interpolate the four results along y
	float nxy0 = Lerpf(nx00, nx10, v);
	float nxy1 = Lerpf(nx01, nx11, v);

	// Interpolate the two last results along z
	float nxyz = Lerpf(nxy0, nxy1, w);

	return nxyz;
}

float PerlinNoiseND(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	// Arrays
	// Get the input from the function with unknown number of parameter
	float* input = new float[nDim];                                       
	
	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, float, input);

	float result = PerlinNoiseNDArray(nDim, input);

	delete[] input;

	return result;
}

float PerlinNoiseNDArray(int nDim, float* input)
{
	if (nDim < 1)
	{
		return 0;
	}

	// Constants that we will use along the function
	const int N_DIM_MINUS_ONE = nDim - 1;
	const int N_DIM_PLUS_ONE = nDim + 1;
	const int NUMBER_OF_GRADIENT_INDICES = 1 << nDim;						// the number of gradient indices is = (2^number_of_dimensions)
	const int NUMBER_OF_EDGES = (nDim > 3)
		? (nDim * (1 << N_DIM_MINUS_ONE))									// the number of edges is = (number_of_dimensions * (2^(number_of_dimensions - 1)))
		: 12;																// special case when nDim is 3 or lower.

	// Arrays
	int* unitGridCells = new int[nDim];                                     // Find unit grid cell containing point
	int* gradientIndices = new int[NUMBER_OF_GRADIENT_INDICES];             // Calculate a set of eight hashed gradient indices
	float* noiseContributions = new float[NUMBER_OF_GRADIENT_INDICES];    // Calculate noise contributions from each of the eight corners
	
	// Find unit grid cell containing point
	for (int i = 0; i < nDim; i++)
	{
		// get the integer part from the input
		unitGridCells[i] = FastFloorf(input[i]);
	}

	// Get relative coordinates of point within that cell
	for (int i = 0; i < nDim; i++)
	{
		// get the fractional part from the input
		input[i] = input[i] - unitGridCells[i];
	}

	// Wrap the integer cells at 255 (smaller integer period can be introduced here)
	for (int i = 0; i < nDim; i++)
	{
		// Because the values from the permutation table are between [0-255] (inclusive), we make sure that we don't overflow.
		unitGridCells[i] &= 255;
	}

	// Calculate a set of eight hashed gradient indices	
	for (int i = 0; i < NUMBER_OF_GRADIENT_INDICES; i++)
	{
		// each index will have the value = (permutation[x + 1_or_0(dependening_of_Index_important_bit) + permutation[y + 1_or_0 + permutation[z + 1_or_0 + ...]]] % number_Of_Indices)
		// x,y,z are the input from a point of 3rd Dimension.
		gradientIndices[i] = 0;
		for (int dim = 0; dim < nDim; dim++)
		{
			gradientIndices[i] = g_HASH_TABLE_KEN_PERLIN[(unitGridCells[(N_DIM_MINUS_ONE - dim)]) +
				CheckBitStatus(i, dim) +
				gradientIndices[i]];

		}
		gradientIndices[i] %= NUMBER_OF_EDGES;
		//
		//noiseContributions[i] = gradientIndices[i];
	}

	
	if (nDim < 4)
	{
		// skip gradient helper because we have a hardcoded one for this
		// and
		// Calculate noise contributions from each corner
		for (int i = 0; i < NUMBER_OF_GRADIENT_INDICES; i++)
		{
			// Here we do a DOT PRODUCT for each corner
			noiseContributions[i] = 0;
			for (int j = 0; j < nDim; j++)
			{
				int scalar = CheckBitStatus(i, N_DIM_MINUS_ONE - j) ? -1 : 0;
				noiseContributions[i] += (input[j] + scalar) * g_GRAD3[gradientIndices[i]][j];
			}
		}
	}
	else
	{
		// Calculate noise contributions from each corner
		for (int i = 0; i < NUMBER_OF_GRADIENT_INDICES; i++)
		{
			// Here we do a DOT PRODUCT for each corner
			noiseContributions[i] = 0;
			int k = 0;
			int perm = i / N_DIM_PLUS_ONE;
			for (int j = 0; j < nDim; j++)
			{
				int dot = perm == j
					? 0
					: CheckBitStatus(i, nDim - 2 - (k++)) == 0 ? 1 : -1;
				int scalar = CheckBitStatus(i, N_DIM_MINUS_ONE - j) ? -1 : 0;
				noiseContributions[i] += (input[j] + scalar) * dot;
			}
		}
	}

	// Compute the fade curve value for each of x, y, z
	float* curves = input;
	for (int i = 0; i < nDim; i++)
	{
		curves[i] = Fadef(input[i]);
	}

	// Interpolate along x the contributions from each of the corners
	int interpolations = NUMBER_OF_GRADIENT_INDICES / 2;
	for (int i = 0; i < nDim; i++)
	{
		for (int j = 0; j < interpolations; j++)
		{
			noiseContributions[j] = Lerpf(noiseContributions[j], noiseContributions[j + interpolations], curves[i]);
		}
		interpolations /= 2;
	}
	float result = noiseContributions[0];

	delete[] unitGridCells;
	delete[] gradientIndices;
	delete[] noiseContributions;

	return result;
}


float* ClassicPerlinNoise1D_Test_Input(float x)
{
	float* result = new float[1];
	result[0] = x;
	return result;
}

float* PerlinNoiseND_Test_Input(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	// Arrays
	float* input = new float[nDim];                                       // Get the input from the function with unknown number of parameter

	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, float, input);
	return input;
}

int* ClassicPerlinNoise1D_Test_UnitGrid(float x)
{
	// Find unit grid cell containing point
	int xi = FastFloorf(x);
	int* result = new int[1];
	result[0] = xi;
	return result;
}


int* PerlinNoiseND_Test_UnitGrid(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	// Arrays
	float* input = new float[nDim];                                       // Get the input from the function with unknown number of parameter

	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, float, input);
	// Constants that we will use along the function
	const int N_DIM_MINUS_ONE = nDim - 1;
	const int N_DIM_PLUS_ONE = nDim + 1;
	const int NUMBER_OF_GRADIENT_INDICES = 1 << nDim;						// the number of gradient indices is = (2^number_of_dimensions)
	const int NUMBER_OF_EDGES = (nDim > 3)
		? (nDim * (1 << N_DIM_MINUS_ONE))									// the number of edges is = (number_of_dimensions * (2^(number_of_dimensions - 1)))
		: 12;																// special case when nDim is 3 or lower.

	// Arrays
	int* unitGridCells = new int[nDim];                                     // Find unit grid cell containing point

	for (int i = 0; i < nDim; i++)
	{
		// get the integer part from the input
		unitGridCells[i] = FastFloorf(input[i]);
	}
	return unitGridCells;
}


float* ClassicPerlinNoise1D_Test_Relative(float x)
{
	// Find unit grid cell containing point
	int xi = FastFloorf(x);

	// Get relative xyz coordinates of point within that cell
	x = x - xi;

	float* result = new float[1];
	result[0] = x;
	return result;
}

float* PerlinNoiseND_Test_Relative(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	// Arrays
	float* input = new float[nDim];                                       // Get the input from the function with unknown number of parameter

	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, float, input);
	// Constants that we will use along the function
	const int N_DIM_MINUS_ONE = nDim - 1;
	const int N_DIM_PLUS_ONE = nDim + 1;
	const int NUMBER_OF_GRADIENT_INDICES = 1 << nDim;						// the number of gradient indices is = (2^number_of_dimensions)
	const int NUMBER_OF_EDGES = (nDim > 3)
		? (nDim * (1 << N_DIM_MINUS_ONE))									// the number of edges is = (number_of_dimensions * (2^(number_of_dimensions - 1)))
		: 12;																// special case when nDim is 3 or lower.

	// Arrays
	int* unitGridCells = new int[nDim];                                     // Find unit grid cell containing point
	int* gradientIndices = new int[NUMBER_OF_GRADIENT_INDICES];             // Calculate a set of eight hashed gradient indices
	int* grad3 = new int[NUMBER_OF_EDGES * nDim];                           // Calculate the gradient helper for noise contributions
	float* noiseContributions = new float[NUMBER_OF_GRADIENT_INDICES];    // Calculate noise contributions from each of the eight corners

	for (int i = 0; i < nDim; i++)
	{
		// get the integer part from the input
		unitGridCells[i] = FastFloorf(input[i]);
	}

	// Get relative coordinates of point within that cell
	for (int i = 0; i < nDim; i++)
	{
		// get the fractional part from the input
		input[i] = input[i] - unitGridCells[i];
	}

	return input;
}


int* ClassicPerlinNoise1D_Test_Wrap(float x)
{
	// Find unit grid cell containing point
	int xi = FastFloorf(x);

	// Get relative xyz coordinates of point within that cell
	x = x - xi;

	// Wrap the integer cells at 255 (smaller integer period can be introduced here)
	xi = xi & 255;

	int* result = new int[1];
	result[0] = xi;
	return result;
}

int* PerlinNoiseND_Test_Wrap(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	// Arrays
	float* input = new float[nDim];                                       // Get the input from the function with unknown number of parameter

	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, float, input);
	// Constants that we will use along the function
	const int N_DIM_MINUS_ONE = nDim - 1;
	const int N_DIM_PLUS_ONE = nDim + 1;
	const int NUMBER_OF_GRADIENT_INDICES = 1 << nDim;						// the number of gradient indices is = (2^number_of_dimensions)
	const int NUMBER_OF_EDGES = (nDim > 3)
		? (nDim * (1 << N_DIM_MINUS_ONE))									// the number of edges is = (number_of_dimensions * (2^(number_of_dimensions - 1)))
		: 12;																// special case when nDim is 3 or lower.

	// Arrays
	int* unitGridCells = new int[nDim];                                     // Find unit grid cell containing point
	int* gradientIndices = new int[NUMBER_OF_GRADIENT_INDICES];             // Calculate a set of eight hashed gradient indices
	int* grad3 = new int[NUMBER_OF_EDGES * nDim];                           // Calculate the gradient helper for noise contributions
	float* noiseContributions = new float[NUMBER_OF_GRADIENT_INDICES];    // Calculate noise contributions from each of the eight corners

	for (int i = 0; i < nDim; i++)
	{
		// get the integer part from the input
		unitGridCells[i] = FastFloorf(input[i]);
	}

	// Get relative coordinates of point within that cell
	for (int i = 0; i < nDim; i++)
	{
		// get the fractional part from the input
		input[i] = input[i] - unitGridCells[i];
	}

	// Wrap the integer cells at 255 (smaller integer period can be introduced here)
	for (int i = 0; i < nDim; i++)
	{
		// Because the values from the permutation table are between [0-255] (inclusive), we make sure that we don't overflow.
		unitGridCells[i] &= 255;
	}

	return unitGridCells;
}

int* ClassicPerlinNoise1D_Test_Gradients(float x)
{
	// Find unit grid cell containing point
	int xi = FastFloorf(x);

	// Get relative xyz coordinates of point within that cell
	x = x - xi;

	// Wrap the integer cells at 255 (smaller integer period can be introduced here)
	xi = xi & 255;

	// Calculate a set of eight hashed gradient indices
	int gi0 = g_HASH_TABLE_KEN_PERLIN[xi] % 12;
	int gi1 = g_HASH_TABLE_KEN_PERLIN[xi + 1] % 12;

	int* gis = new int[2];
	gis[0] = gi0;
	gis[1] = gi1;
	return gis;
}

int* PerlinNoiseNDArray_Test_Gradients(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	float* input = new float[nDim];

	VA_LIST_GET(nDim, float, input);

	const int N_DIM_MINUS_ONE = nDim - 1;
	const int NUMBER_OF_GRADIENT_INDICES = 1 << nDim;
	const int NUMBER_OF_EDGES = (nDim > 3)
		? (nDim * (1 << N_DIM_MINUS_ONE))
		: 12;

	int* unitGridCells = new int[nDim];
	int* gradientIndices = new int[NUMBER_OF_GRADIENT_INDICES];
	int* grad3 = new int[NUMBER_OF_EDGES * nDim];

	for (int i = 0; i < nDim; i++)
	{
		unitGridCells[i] = FastFloorf(input[i]);
	}

	for (int i = 0; i < nDim; i++)
	{
		input[i] = input[i] - unitGridCells[i];
	}

	for (int i = 0; i < nDim; i++)
	{
		unitGridCells[i] &= 255;
	}

	for (int i = 0; i < NUMBER_OF_GRADIENT_INDICES; i++)
	{
		gradientIndices[i] = 0;
		for (int dim = 0; dim < nDim; dim++)
		{
			gradientIndices[i] = g_HASH_TABLE_KEN_PERLIN[(unitGridCells[(N_DIM_MINUS_ONE - dim)]) +
				CheckBitStatus(i, dim) +
				gradientIndices[i]];

		}
		gradientIndices[i] %= NUMBER_OF_EDGES;
	}
	return gradientIndices;
}


