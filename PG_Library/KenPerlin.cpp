#include "pch.h"
#include "KenPerlin.h"

#include "Math.h"
#include "Constants.h"
#include "Allocation.h"
#include "PerlinUtils.h"



double ClassicPerlinNoise1D(double x)
{
	// Find unit grid cell containing point
	int xi = FastFloor(x);

	// Get relative xyz coordinates of point within that cell
	x = x - xi;

	// Wrap the integer cells at 255 (smaller integer period can be introduced here)
	xi = xi & 255;

	// Calculate a set of eight hashed gradient indices
	int gi0 = g_HASH_TABLE_KEN_PERLIN[xi] % 12;
	int gi1 = g_HASH_TABLE_KEN_PERLIN[xi + 1] % 12;

	// Calculate noise contributions from each of the eight corners
	double n0 = Dot(g_GRAD3[gi0][0], x);
	double n1 = Dot(g_GRAD3[gi1][0], x - 1);

	// Compute the fade curve value for each of x, y, z
	double u = Fade(x);

	// Interpolate along x the contributions from each of the corners
	double nx = Lerpd(n0, n1, u);

	return nx;
}

double ClassicPerlinNoise2D(double x, double y)
{
	// Find unit grid cell containing point
	int xi = FastFloor(x);
	int yi = FastFloor(y);

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

	// Calculate noise contributions from each of the eight corners
	double n00 = DotN(2,g_GRAD3[gi00], x, y);
	double n01 = DotN(2,g_GRAD3[gi01], x, y - 1);
	double n10 = DotN(2,g_GRAD3[gi10], x - 1, y);
	double n11 = DotN(2,g_GRAD3[gi11], x - 1, y - 1);

	// Compute the fade curve value for each of x, y, z
	double u = Fade(x);
	double v = Fade(y);

	// Interpolate along x the contributions from each of the corners
	double nx0 = Lerpd(n00, n10, u);
	double nx1 = Lerpd(n01, n11, u);

	// Interpolate the four results along y
	double nxy = Lerpd(nx0, nx1, v);

	return nxy;
}

double ClassicPerlinNoise3D(double x, double y, double z)
{
	// Find unit grid cell containing point
	int xi = FastFloor(x);
	int yi = FastFloor(y);
	int zi = FastFloor(z);

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

	// Calculate noise contributions from each of the eight corners
	double n000 = DotN(3,g_GRAD3[gi000], x, y, z);
	double n001 = DotN(3,g_GRAD3[gi001], x, y, z - 1);
	double n010 = DotN(3,g_GRAD3[gi010], x, y - 1, z);
	double n011 = DotN(3,g_GRAD3[gi011], x, y - 1, z - 1);
	double n100 = DotN(3,g_GRAD3[gi100], x - 1, y, z);
	double n101 = DotN(3,g_GRAD3[gi101], x - 1, y, z - 1);
	double n110 = DotN(3,g_GRAD3[gi110], x - 1, y - 1, z);
	double n111 = DotN(3,g_GRAD3[gi111], x - 1, y - 1, z - 1);

	// Compute the fade curve value for each of x, y, z
	double u = Fade(x);
	double v = Fade(y);
	double w = Fade(z);

	// Interpolate along x the contributions from each of the corners
	double nx00 = Lerpd(n000, n100, u);
	double nx01 = Lerpd(n001, n101, u);
	double nx10 = Lerpd(n010, n110, u);
	double nx11 = Lerpd(n011, n111, u);

	// Interpolate the four results along y
	double nxy0 = Lerpd(nx00, nx10, v);
	double nxy1 = Lerpd(nx01, nx11, v);

	// Interpolate the two last results along z
	double nxyz = Lerpd(nxy0, nxy1, w);

	return nxyz;
}



double PerlinNoiseND(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	// Arrays
	double* input = new double[nDim];                                       // Get the input from the function with unknown number of parameter
	
	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, double, input);

	double result = PerlinNoiseNDArray(nDim, input);

	delete[] input;

	return result;
}

double PerlinNoiseNDArray(int nDim, double* input)
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
	double* noiseContributions = new double[NUMBER_OF_GRADIENT_INDICES];    // Calculate noise contributions from each of the eight corners
	
	// Find unit grid cell containing point
	for (int i = 0; i < nDim; i++)
	{
		// get the integer part from the input
		unitGridCells[i] = FastFloor(input[i]);
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
	double* curves = input;
	for (int i = 0; i < nDim; i++)
	{
		curves[i] = Fade(input[i]);
	}

	// Interpolate along x the contributions from each of the corners
	int interpolations = NUMBER_OF_GRADIENT_INDICES / 2;
	for (int i = 0; i < nDim; i++)
	{
		for (int j = 0; j < interpolations; j++)
		{
			noiseContributions[j] = Lerpd(noiseContributions[j], noiseContributions[j + interpolations], curves[i]);
		}
		interpolations /= 2;
	}
	double result = noiseContributions[0];

	delete[] unitGridCells;
	delete[] gradientIndices;
	delete[] noiseContributions;

	return result;
}


double* ClassicPerlinNoise1D_Test_Input(double x)
{
	double* result = new double[1];
	result[0] = x;
	return result;
}

double* PerlinNoiseND_Test_Input(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	// Arrays
	double* input = new double[nDim];                                       // Get the input from the function with unknown number of parameter

	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, double, input);
	return input;
}

int* ClassicPerlinNoise1D_Test_UnitGrid(double x)
{
	// Find unit grid cell containing point
	int xi = FastFloor(x);
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
	double* input = new double[nDim];                                       // Get the input from the function with unknown number of parameter

	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, double, input);
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
		unitGridCells[i] = FastFloor(input[i]);
	}
	return unitGridCells;
}


double* ClassicPerlinNoise1D_Test_Relative(double x)
{
	// Find unit grid cell containing point
	int xi = FastFloor(x);

	// Get relative xyz coordinates of point within that cell
	x = x - xi;

	double* result = new double[1];
	result[0] = x;
	return result;
}

double* PerlinNoiseND_Test_Relative(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	// Arrays
	double* input = new double[nDim];                                       // Get the input from the function with unknown number of parameter

	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, double, input);
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
	double* noiseContributions = new double[NUMBER_OF_GRADIENT_INDICES];    // Calculate noise contributions from each of the eight corners

	for (int i = 0; i < nDim; i++)
	{
		// get the integer part from the input
		unitGridCells[i] = FastFloor(input[i]);
	}

	// Get relative coordinates of point within that cell
	for (int i = 0; i < nDim; i++)
	{
		// get the fractional part from the input
		input[i] = input[i] - unitGridCells[i];
	}

	return input;
}


int* ClassicPerlinNoise1D_Test_Wrap(double x)
{
	// Find unit grid cell containing point
	int xi = FastFloor(x);

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
	double* input = new double[nDim];                                       // Get the input from the function with unknown number of parameter

	// Get the input from the function with unknown number of parameters
	VA_LIST_GET(nDim, double, input);
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
	double* noiseContributions = new double[NUMBER_OF_GRADIENT_INDICES];    // Calculate noise contributions from each of the eight corners

	for (int i = 0; i < nDim; i++)
	{
		// get the integer part from the input
		unitGridCells[i] = FastFloor(input[i]);
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

int* ClassicPerlinNoise1D_Test_Gradients(double x)
{
	// Find unit grid cell containing point
	int xi = FastFloor(x);

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

	double* input = new double[nDim];

	VA_LIST_GET(nDim, double, input);

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
		unitGridCells[i] = FastFloor(input[i]);
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


