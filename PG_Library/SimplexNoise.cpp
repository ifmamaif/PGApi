#include "pch.h"
#include "SimplexNoise.h"

#include "Math.h"
#include "Constants.h"
#include "Allocation.h"
#include "PerlinUtils.h"

// ------------------------------------------------------------------------------------------------
//   A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D).
// 
// This C++ implementation is based on the speed-improved Java version 2012-03-09
// by Stefan Gustavson (original Java source code in the public domain).
// - Based on example code by Stefan Gustavson (stegu@itn.liu.se).
// - Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
// - Better rank ordering method by Stefan Gustavson in 2012.
// 
// This implementation is "Simplex Noise" as presented by
// Ken Perlin at a relatively obscure and not often cited course
// session "Real-Time Shading" at Siggraph 2001 (before real
// time shading actually took on), under the title "hardware noise".
// The 3D function is numerically equivalent to his Java reference
// code available in the PDF course notes, although I re-implemented
// it from scratch to get more readable code. The 1D, 2D and 4D cases
// were implemented from scratch by me from Ken Perlin's text.
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Simplex noise
// � Simplex noise has a lower computational complexity and requires fewer multiplications.
// � Simplex noise scales to higher dimensions(4D, 5D and up) with much less computational
//		cost, the complexity is for dimensions instead of the of classic Noise.
// � Simplex noise has no noticeable directional artifacts.
// � Simplex noise has a well - defined and continuous gradient everywhere that can be computed
//		quite cheaply.
// � Simplex noise is easy to implement in hardware.
// ------------------------------------------------------------------------------------------------

// A lookup table to traverse the simplex around a given point in 4D.
// Details can be found where this table is used, in the 4D noise method.
static const int SIMPLEX_TABLE[64][4] =
{
	{0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
	{0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
	{1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
	{2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}
};

static const int grad4[32][4] =
{
	{0,1,1,1}	, {0,1,1,-1}, {0,1,-1,1}, {0,1,-1,-1},
	{0,-1,1,1}	, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
	{1,0,1,1}	, {1,0,1,-1}, {1,0,-1,1}, {1,0,-1,-1},
	{-1,0,1,1}	, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
	{1,1,0,1}	, {1,1,0,-1}, {1,-1,0,1}, {1,-1,0,-1},
	{-1,1,0,1}	, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
	{1,1,1,0}	, {1,1,-1,0}, {1,-1,1,0}, {1,-1,-1,0},
	{-1,1,1,0}	, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}
};

float ContributionFromCorners(const int nInput, const int gradient[], ...)
{
	float* input = new float[nInput];
	VA_LIST_GET(nInput, float, input);
	// The scalar should be 0.5, not 0.6, else the noise is not continuous at simplex boundaries.
	float t = 0.5;
	for (int i = 0; i < nInput; i++)
	{
		t -= input[i] * input[i];
	}

	if (t < 0)
	{
		return 0.0;
	}

	t *= t;
	float result = t * t * DotArray(nInput, gradient, input);
	//float result = t * t * Gradient2D(nInput, gradient, input);
	delete[] input;
	return result;
}

float SimplexNoise1D(float x) {
	float n0, n1;   // Noise contributions from the two "corners"

	// No need to skew the input space in 1D

	// Corners coordinates (nearest integer values):
	int i0 = FastFloorf(x);
	int i1 = i0 + 1;
	// Distances to corners (between 0 and 1):
	float x0 = x - i0;
	float x1 = x0 - 1.0f;

	// Calculate the contribution from the first corner
	float t0 = 1.0f - x0 * x0;
	//  if(t0 < 0.0f) t0 = 0.0f; // not possible
	t0 *= t0;
	n0 = t0 * t0 * Gradient1D(g_HASH_TABLE_KEN_PERLIN[i0], x0);

	// Calculate the contribution from the second corner
	float t1 = 1.0f - x1 * x1;
	//  if(t1 < 0.0f) t1 = 0.0f; // not possible
	t1 *= t1;
	n1 = t1 * t1 * Gradient1D(g_HASH_TABLE_KEN_PERLIN[i1], x1);

	// The maximum value of this noise is 8*(3/4)^4 = 2.53125
	// A factor of 0.395 scales to fit exactly within [-1,1]
	return 0.395f * (n0 + n1);
}

// 2D simplex noise
float SimplexNoise2D(float xin, float yin)
{
	// Skewing factors for 2D
	const float F2 = 0.36602540378; // 0.5 * (sqrt(3.0) - 1.0);

	// Skew the input space to determine which simplex cell we're in
	float s = (xin + yin) * F2; // Hairy factor for 2D
	int i = FastFloorf(xin + s);
	int j = FastFloorf(yin + s);

	// Unskewing factors for 2D
	const float G2 = 0.2113248654; // (3.0 - sqrt(3.0)) / 6.0;
	const float t = (i + j) * G2;

	// Unskew the cell origin back to (x,y) space
	float x0 = i - t;
	float y0 = j - t;
	// The x,y distances from the cell origin
	x0 = xin - x0;
	y0 = yin - y0;

	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
	// Offsets for second (middle) corner of simplex in (i,j) coords
	int i1, j1;
	// lower triangle, XY order: (0,0)->(1,0)->(1,1)
	if (x0 > y0)
	{
		i1 = 1;
		j1 = 0;
	}
	// upper triangle, YX order: (0,0)->(0,1)->(1,1)
	else
	{
		i1 = 0;
		j1 = 1;
	}

	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6

	// Offsets for middle corner in (x,y) unskewed coords
	float x1 = x0 - i1 + G2;
	float y1 = y0 - j1 + G2;
	// Offsets for last corner in (x,y) unskewed coords
	float x2 = x0 - 1.0 + 2.0 * G2;
	float y2 = y0 - 1.0 + 2.0 * G2;

	// Work out the hashed gradient indices of the three simplex corners
	i = i & 255;
	j = j & 255;
	int gi0 = g_HASH_TABLE_KEN_PERLIN[i + g_HASH_TABLE_KEN_PERLIN[j]] % 12;
	int gi1 = g_HASH_TABLE_KEN_PERLIN[i + i1 + g_HASH_TABLE_KEN_PERLIN[j + j1]] % 12;
	int gi2 = g_HASH_TABLE_KEN_PERLIN[i + 1 + g_HASH_TABLE_KEN_PERLIN[j + 1]] % 12;

	// Noise contributions from the three corners
	float n0, n1, n2;
	// Calculate the contribution from the three corners
	n0 = ContributionFromCorners(2, grad4[gi0], x0, y0);
	n1 = ContributionFromCorners(2, grad4[gi1], x1, y1);
	n2 = ContributionFromCorners(2, grad4[gi2], x2, y2);

	// Add contributions from each corner to get the const noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 70.0 * (n0 + n1 + n2);
}

// 3D simplex noise
float SimplexNoise3D(float xin, float yin, float zin)
{
	// Skew the input space to determine which simplex cell we're in
	const float F3 = 1.0 / 3.0;
	// Very nice and simple skew factor for 3D
	float s = (xin + yin + zin) * F3;
	int i = FastFloorf(xin + s);
	int j = FastFloorf(yin + s);
	int k = FastFloorf(zin + s);

	// Very nice and simple unskew factor, too
	const float G3 = 1.0 / 6.0;
	const float t = (i + j + k) * G3;

	// Unskew the cell origin back to (x,y,z) space
	float x0 = i - t;
	float y0 = j - t;
	float z0 = k - t;

	// The x,y,z distances from the cell origin
	x0 = xin - x0;
	y0 = yin - y0;
	z0 = zin - z0;

	// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
	// Determine which simplex we are in.
	int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
	int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

	if (x0 >= y0)
	{
		if (y0 >= z0)			// x0 >= y0 >= z0
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		} // X Y Z order
		else if (x0 >= z0)		// x0 >= y0, z0
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1;
		} // X Z Y order
		else // x0 < z0			// x0 >= y0, x0<z0
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1;
		} // Z X Y order
	}
	else
	{ // x0<y0
		if (y0 < z0)			// x0 < y0 < z0
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1;
		} // Z Y X order
		else if (x0 < z0)		// x0 < y0,z0
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;
		} // Y Z X order
		else					// x0<y0, x0>=z0
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		} // Y X Z order
	}

	// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
	// a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
	// a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
	// c = 1/6.

	// Offsets for second corner in (x,y,z) coordinates
	float x1 = x0 - i1 + G3;
	float y1 = y0 - j1 + G3;
	float z1 = z0 - k1 + G3;

	// Offsets for third corner in (x,y,z) coordinates
	float x2 = x0 - i2 + 2.0 * G3;
	float y2 = y0 - j2 + 2.0 * G3;
	float z2 = z0 - k2 + 2.0 * G3;

	// Offsets for last corner in (x,y,z) coordinates
	float x3 = x0 - 1.0 + 3.0 * G3;
	float y3 = y0 - 1.0 + 3.0 * G3;
	float z3 = z0 - 1.0 + 3.0 * G3;

	// Work out the hashed gradient indices of the four simplex corners
	i = i & 255;
	j = j & 255;
	k = k & 255;
	int gi0 = g_HASH_TABLE_KEN_PERLIN[i + g_HASH_TABLE_KEN_PERLIN[j + g_HASH_TABLE_KEN_PERLIN[k]]] % 12;
	int gi1 = g_HASH_TABLE_KEN_PERLIN[i + i1 + g_HASH_TABLE_KEN_PERLIN[j + j1 + g_HASH_TABLE_KEN_PERLIN[k + k1]]] % 12;
	int gi2 = g_HASH_TABLE_KEN_PERLIN[i + i2 + g_HASH_TABLE_KEN_PERLIN[j + j2 + g_HASH_TABLE_KEN_PERLIN[k + k2]]] % 12;
	int gi3 = g_HASH_TABLE_KEN_PERLIN[i + 1 + g_HASH_TABLE_KEN_PERLIN[j+ 1 + g_HASH_TABLE_KEN_PERLIN[k + 1]]] % 12;

	// Noise contributions from the four corners
	float n0, n1, n2, n3;
	// Calculate the contribution from the four corners	
	n0 = ContributionFromCorners(3, g_GRAD3[gi0], x0, y0, z0);
	n1 = ContributionFromCorners(3, g_GRAD3[gi1], x1, y1, z1);
	n2 = ContributionFromCorners(3, g_GRAD3[gi2], x2, y2, z2);
	n3 = ContributionFromCorners(3, g_GRAD3[gi3], x3, y3, z3);

	// Add contributions from each corner to get the const noise value.
	// The result is scaled to stay just inside [-1,1]
	return 32.0 * (n0 + n1 + n2 + n3);
}

// 4D simplex noise
float SimplexNoise4D(float x, float y, float z, float w) {

	// The skewing and unskewing factors are hairy again for the 4D case
	const float F4 = 0.30901699437; // (sqrt(5.0) - 1.0) / 4.0;
	const float G4 = 0.13819660112; // (5.0 - sqrt(5.0)) / 20.0;	

	// Skew the (x,y,z,w) space to determine which cell of 24 simplicities we are in
	const float s = (x + y + z + w) * F4; // Factor for 4D skewing
	int i = FastFloor(x + s);
	int j = FastFloorf(y + s);
	int k = FastFloorf(z + s);
	int l = FastFloorf(w + s);

	const float t = (i + j + k + l) * G4; // Factor for 4D unskewing
	float x0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
	float y0 = j - t;
	float z0 = k - t;
	float w0 = l - t;

	x0 = x - x0; // The x,y,z,w distances from the cell origin
	y0 = y - y0;
	z0 = z - z0;
	w0 = w - w0;

	// For the 4D case, the simplex is a 4D shape I won't even try to describe.
	// To find out which of the 24 possible simplicities we're in, we need to
	// determine the magnitude ordering of x0, y0, z0 and w0.
	// The method below is a good way of finding the ordering of x,y,z,w and
	// then find the correct traversal order for the simplex we are in.
	// First, six pair-wise comparisons are performed between each possible pair
	// of the four coordinates, and the results are used to add up binary bits
	// for an integer index.
	const int c1 = (x0 > y0) ? 32 : 0;
	const int c2 = (x0 > z0) ? 16 : 0;
	const int c3 = (y0 > z0) ? 8 : 0;
	const int c4 = (x0 > w0) ? 4 : 0;
	const int c5 = (y0 > w0) ? 2 : 0;
	const int c6 = (z0 > w0) ? 1 : 0;
	const int c = c1 + c2 + c3 + c4 + c5 + c6;

	int i1, j1, k1, l1; // The integer offsets for the second simplex corner
	int i2, j2, k2, l2; // The integer offsets for the third simplex corner
	int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner

	// simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
	// Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
	// impossible. Only the 24 indices which have non-zero entries make any sense.
	// We use a thresholding to set the coordinates in turn from the largest magnitude.
	// The number 3 in the "simplex" array is at the position of the largest coordinate.
	i1 = SIMPLEX_TABLE[c][0] >= 3 ? 1 : 0;
	j1 = SIMPLEX_TABLE[c][1] >= 3 ? 1 : 0;
	k1 = SIMPLEX_TABLE[c][2] >= 3 ? 1 : 0;
	l1 = SIMPLEX_TABLE[c][3] >= 3 ? 1 : 0;
	// The number 2 in the "simplex" array is at the second largest coordinate.
	i2 = SIMPLEX_TABLE[c][0] >= 2 ? 1 : 0;
	j2 = SIMPLEX_TABLE[c][1] >= 2 ? 1 : 0;
	k2 = SIMPLEX_TABLE[c][2] >= 2 ? 1 : 0;
	l2 = SIMPLEX_TABLE[c][3] >= 2 ? 1 : 0;
	// The number 1 in the "simplex" array is at the second smallest coordinate.
	i3 = SIMPLEX_TABLE[c][0] >= 1 ? 1 : 0;
	j3 = SIMPLEX_TABLE[c][1] >= 1 ? 1 : 0;
	k3 = SIMPLEX_TABLE[c][2] >= 1 ? 1 : 0;
	l3 = SIMPLEX_TABLE[c][3] >= 1 ? 1 : 0;

	// The fifth corner has all coordinate offsets = 1, so no need to look that up.
	// Offsets for second corner in (x,y,z,w) coordinates
	const float x1 = x0 - i1 + G4;
	const float y1 = y0 - j1 + G4;
	const float z1 = z0 - k1 + G4;
	const float w1 = w0 - l1 + G4;

	// Offsets for third corner in (x,y,z,w) coordinates
	const float x2 = x0 - i2 + 2.0 * G4;
	const float y2 = y0 - j2 + 2.0 * G4;
	const float z2 = z0 - k2 + 2.0 * G4;
	const float w2 = w0 - l2 + 2.0 * G4;

	// Offsets for fourth corner in (x,y,z,w) coordinates
	const float x3 = x0 - i3 + 3.0 * G4;
	const float y3 = y0 - j3 + 3.0 * G4;
	const float z3 = z0 - k3 + 3.0 * G4;
	const float w3 = w0 - l3 + 3.0 * G4;

	// Offsets for last corner in (x,y,z,w) coordinates
	const float x4 = x0 - 1.0 + 4.0 * G4;
	const float y4 = y0 - 1.0 + 4.0 * G4;
	const float z4 = z0 - 1.0 + 4.0 * G4;
	const float w4 = w0 - 1.0 + 4.0 * G4;

	// Work out the hashed gradient indices of the five simplex corners
	i = i & 255;
	j = j & 255;
	k = k & 255;
	l = l & 255;
	const int gi0 = g_HASH_TABLE_KEN_PERLIN[i + g_HASH_TABLE_KEN_PERLIN[j + g_HASH_TABLE_KEN_PERLIN[k + g_HASH_TABLE_KEN_PERLIN[l]]]] % 32;
	const int gi1 = g_HASH_TABLE_KEN_PERLIN[i + i1 + g_HASH_TABLE_KEN_PERLIN[j + j1 + g_HASH_TABLE_KEN_PERLIN[k + k1 + g_HASH_TABLE_KEN_PERLIN[l + l1]]]] % 32;
	const int gi2 = g_HASH_TABLE_KEN_PERLIN[i + i2 + g_HASH_TABLE_KEN_PERLIN[j + j2 + g_HASH_TABLE_KEN_PERLIN[k + k2 + g_HASH_TABLE_KEN_PERLIN[l + l2]]]] % 32;
	const int gi3 = g_HASH_TABLE_KEN_PERLIN[i + i3 + g_HASH_TABLE_KEN_PERLIN[j + j3 + g_HASH_TABLE_KEN_PERLIN[k + k3 + g_HASH_TABLE_KEN_PERLIN[l + l3]]]] % 32;
	const int gi4 = g_HASH_TABLE_KEN_PERLIN[i + 1 + g_HASH_TABLE_KEN_PERLIN[j + 1 + g_HASH_TABLE_KEN_PERLIN[k + 1 + g_HASH_TABLE_KEN_PERLIN[l + 1]]]] % 32;

	// Noise contributions from the five corners
	float n0, n1, n2, n3, n4;
	// Calculate the contribution from the five corners
	n0 = ContributionFromCorners(4, grad4[gi0], x0, y0, z0, w0);
	n1 = ContributionFromCorners(4, grad4[gi1], x1, y1, z1, w1);
	n2 = ContributionFromCorners(4, grad4[gi2], x2, y2, z2, w2);
	n3 = ContributionFromCorners(4, grad4[gi3], x3, y3, z3, w3);
	n4 = ContributionFromCorners(4, grad4[gi4], x4, y4, z4, w4);

	// Sum up and scale the result to cover the range [-1,1]
	return 27.0 * (n0 + n1 + n2 + n3 + n4);
}


//float SimplexNoiseND(int nDim, ...)
//{
//	if (nDim < 1)
//	{
//		return 0; 
//	}
//
//	// Get the input from the function with unknown number of parameters
//	float* input = new float[nDim];
//	VA_LIST_GET(nDim, float, input);
//
//	// The skewing and unskewing factors are hairy again for the N-D case
//	// SKEWING
//	const float SKEWING = (sqrt(nDim + 1) - 1) / nDim;
//	// UNSKEWING
//	const float UNSKEWING = (1 - 1 / (sqrt(nDim+1))) / nDim;
//	
//	// Factor for N-D skewing
//	float s = 0;
//	for (int i = 0; i < nDim; i++)
//	{
//		s += input[i];
//	}
//	s *= SKEWING; 
//	// Skew the (x,y,z,...) space to determine which cell of the simplicities we are in
//	int* cellSkewed = new int[nDim];
//	for (int i = 0; i < nDim; i++)
//	{
//		cellSkewed[i] = FastFloorf(input[i] + s);
//	}
//
//	// Factor for N-D unskewing
//	float t = 0;
//	for (int i = 0; i < nDim; i++)
//	{
//		t += input[i];
//	}
//	t *= UNSKEWING;
//	// Unskew the cell origin back to (x,y,z,...) space
//	int* cellUnskewed = new int[nDim];
//	for (int i = 0; i < nDim; i++)
//	{
//		cellUnskewed[i] = (int)(cellSkewed[i] - t);
//	}
//
//	// The x,y,z,w distances from the cell origin
//	for (int i=0;i<nDim;i++)
//	{
//		cellUnskewed[i] = (int)(input[i] - cellUnskewed[i]);
//	}
//
//	// To find out which of the possible simplicities we're in, we need to
//	// determine the magnitude ordering of x0, y0, z0 and ...
//	// Pair-wise comparisons are performed between each possible pair
//	// of coordinates, and the results are used to rank the numbers.
//	int* magnitudes = new int[nDim];
//	for (int i = 0; i < nDim; i++)
//	{
//		for (int j = i + 1; j < nDim; j++)
//		{
//			if (cellUnskewed[i] > cellUnskewed[j])
//			{
//				magnitudes[i]++;
//			}
//			else
//			{
//				magnitudes[j]++;
//			}
//		}
//	}
//
//	// The integer offsets for the second,third,fourth,... simplex corner
//	// [rankx, ranky, rankz, ....] is a N-vector
//    // in some order. We use a thresholding to set the coordinates in turn.
//	int** simplexCorners = new int* [nDim];
//	for (int i = 0; i < nDim-1; i++)
//	{
//		simplexCorners[i] = new int[nDim];
//		for (int j = 0; j < nDim; j++)
//		{
//			simplexCorners[i][j] = magnitudes[j] >= (3-i) ? 1 : 0;
//		}
//	}
//	for (int j = 0; j < nDim; j++)
//	{
//		simplexCorners[nDim-1][j] = 1;
//	}
//
//
//	// The final corner has all coordinate offsets = 1, so no need to compute that.
//	float** finalCorner = new float* [nDim];
//	for (int i = 0; i < nDim; i++)
//	{
//		finalCorner[i] = new float[nDim];
//		for (int j = 0; j < nDim; j++)
//		{
//			finalCorner[i][j] = cellUnskewed[j] - simplexCorners[i][j] + (i + 1) * UNSKEWING;
//		}
//	}
//
//	// Work out the hashed gradient indices of the five simplex corners
//	for (int i = 0; i < nDim; i++)
//	{
//		cellSkewed[i] &= 255;
//	}
//
//	int* gradients = new int[nDim + 1];
//	gradients[0] = 0;
//	for (int i = nDim - 1; i >= 0; i--)
//	{
//		gradients[0] += permutation[cellSkewed[i] + gradients[0]];
//	}
//	gradients[0] %= 32;
//	for (int i = 0; i < nDim; i++)
//	{
//		gradients[i+1] = 0;
//		for (int j = 0; j < nDim; j++)
//		{
//			gradients[i+1] += permutation[(int)(cellSkewed[i] + finalCorner[i] + gradients[i+1])];
//		}
//		gradients[i + 1] = 0;
//	}
//		
//	// Calculate the contribution from the five corners
//	float* contributions = new float[nDim + 1];
//	// Noise contributions from the five corners
//	float* noise = new float[nDim + 1];
//
//
//	contributions[0] = 0.6;
//	for (int j = 0; j < nDim; j++)
//	{
//		contributions[0] -= cellUnskewed[j] * cellUnskewed[j];
//	}
//	if (contributions[0] < 0)
//	{
//		noise[0] = 0;
//	}
//	else
//	{
//		contributions[0] *= contributions[0];
//		noise[0] = contributions[0] * contributions[0] * DotN(nDim,gradients, cellUnskewed);
//	}
//	for (int i = 1; i < nDim + 1; i++)
//	{
//		contributions[i] = 0.6;
//		for (int j = 0; j < nDim; j++)
//		{
//			contributions[i] -= cellUnskewed[j] * cellUnskewed[j];
//		}
//		if (contributions[i] < 0)
//		{
//			noise[i] = 0;
//		}
//		else
//		{
//			contributions[i] *= contributions[i];
//			noise[i] = contributions[i] * contributions[i] * DotN(nDim, gradients, finalCorner[i]);
//		}
//	}
//
//	const float someConstant = 70;
//	return someConstant * Sum(noise);
//}