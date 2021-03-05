#include "pch.h"
#include "KenPerlin.h"

#include "Math.h"
#include "Constants.h"
#include "Allocation.h"



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

double Gradient(int hash, double x, double y, double z)
{
	int h = hash & 15;                                          // Take the hashed value and take the first 4 bits of it (15 == 0b1111)
	double u = h < 8 /* 0b1000 */ ? x : y;                      // If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.

	double v;                                                   // In Ken Perlin's original implementation this was another conditional operator (?:).  I
																// expanded it for readability.

	if (h < 4 /* 0b0100 */)                                     // If the first and second significant bits are 0 set v = y
		v = y;
	else if (h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)       // If the first and second significant bits are 1 set v = x
		v = x;
	else                                                        // If the first and second significant bits are not equal (0/1, 1/0) set v = z
		v = z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);   // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
}

double Fade(double t)
{
	// Fade function as defined by Ken Perlin.  
	// This eases coordinate values so that they will "ease" towards integral values. 
	// This ends up smoothing the const output.
	// 6*t^5 - 15*t^4 + 10*t^3
	return t * t * t * (t * (t * 6 - 15) + 10);     
}

double MutationValue(double x)
{
	//return (x + 1) / 2;					     	// For convenience we bound it to 0 - 1 (theoretical min/max before is -1 - 1)
	return (x + 0.7) / 1.5;                         // Magic value to obtain [-0.2,1.13] 
}

double OctavePerlin(double x, double y, double z, int octaves, double persistence)
{
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	double maxValue = 0;                            // Used for normalizing result to 0.0 - 1.0
	for (int i = 0; i < octaves; i++)
	{
		total += PerlinNoise3DUnity(x * frequency, y * frequency, z * frequency) * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		frequency *= 2;
	}

	return total / maxValue;
}

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

double PerlinNoise3DUnity(double x, double y, double z)
{
	const int xi = (int)x & 255;                              // Calculate the "unit cube" that the point asked will be located in
	const int yi = (int)y & 255;                              // The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
	const int zi = (int)z & 255;                              // plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.
	const double xf = x - (int)x;                             // We also fade the location to smooth the result.
	const double yf = y - (int)y;
	const double zf = z - (int)z;
	const double u = Fade(xf);
	const double v = Fade(yf);
	const double w = Fade(zf);

	int aaa = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi] + yi] + zi];
	int aba = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi] + yi + 1] + zi];
	int aab = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi] + yi] + zi + 1];
	int abb = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi] + yi + 1] + zi + 1];
	int baa = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi] + zi];
	int bba = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi + 1] + zi];
	int bab = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi] + zi + 1];
	int bbb = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi + 1] + zi + 1];

	double x1, x2, y1, y2;
	x1 = Lerpd(Gradient(aaa, xf, yf, zf),					// The gradient function calculates the dot product between a pseudo random
		      Gradient(baa, xf - 1, yf, zf),				// gradient vector and the vector from the input coordinate to the 8
		      u);											// surrounding points in its unit cube.
	x2 = Lerpd(Gradient(aba, xf, yf - 1, zf),				// This is all then lerped together as a sort of weighted average based on the faded (u,v,w)
		      Gradient(bba, xf - 1, yf - 1, zf),			// values we made earlier.
		      u);
	y1 = Lerpd(x1, x2, v);
	x1 = Lerpd(Gradient(aab, xf, yf, zf - 1),
			  Gradient(bab, xf - 1, yf, zf - 1),
			  u);
	x2 = Lerpd(Gradient(abb, xf, yf - 1, zf - 1),
			  Gradient(bbb, xf - 1, yf - 1, zf - 1),
			  u);
	y2 = Lerpd(x1, x2, v);

	return MutationValue(Lerpd(y1, y2, w));
}

double PerlinNoise2DUnity(double x, double y)
{
	const int xi = (int)x & 255;
	const int yi = (int)y & 255;
	const double xf = x - (int)x;
	const double yf = y - (int)y;
	const double u = Fade(xf);
	const double v = Fade(yf);

	const int aaa = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi] + yi]];
	const int aba = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi] + yi + 1]];
	const int baa = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi]];
	const int bba = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi + 1]];

	double x1, x2;
	x1 = Lerpd(Gradient(aaa, xf, yf, 0),
		Gradient(baa, xf - 1, yf, 0),
		u);
	x2 = Lerpd(Gradient(aba, xf, yf - 1, 0),
		Gradient(bba, xf - 1, yf - 1, 0),
		u);

	return MutationValue(Lerpd(x1, x2, v));
}

double PerlinNoiseUnity(double x)
{
	const int xi = (int)x & 255;
	const double xf = x - (int)x;

	const int aaa = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi]]];
	const int baa = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1]]];

	return MutationValue(Lerpd(Gradient(aaa, xf, 0, 0),
		Gradient(baa, xf - 1, 0, 0),
		Fade(xf)));
}

double** Generate2DMap(int width, int height, float scale, float offsetX, float offsetY)
{
	//double** noiseMap = (double**)malloc(sizeof(double*)*width);
	//for (int i = 0; i < width; i++)
	//{
	//	noiseMap[i] = (double*)malloc(sizeof(double)*height);
	//}
	double** noiseMap = new double*[width];
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
			noiseMap[x][y] = (double)PerlinNoise3DUnity(xCoord, yCoord, 0.f);
		}
	}
	return noiseMap;
}

double& Get2DMapValue(double** buffer, int width, int height)
{
	return buffer[width][height];
}

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
static const int simplex[64][4] = 
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

// Calculate the contribution from the three corners
double ContributionFromCorner3(const int gradient[], double x, double y)
{
	double t = 0.5 - x * x - y * y;

	if (t < 0)
	{
		return 0.0;
	}

	t *= t;
	return t * t * Dot2(gradient, x, y);
}

// Calculate the contribution from the four corners
double ContributionFromCorner4(const int gradient[], double x, double y, double z)
{
	// The scalar should be 0.5, not 0.6, else the noise is not continuous at simplex boundaries.
	double t = 0.5 - x * x - y * y - z * z;

	if (t < 0)
	{
		return 0.0;
	}

	t *= t;
	return t * t * Dot3(gradient, x, y, z);
}

// Calculate the contribution from the five corners
double ContributionFromCorner5(const int gradient[], double x, double y, double z, double w)
{
	// The scalar should be 0.5, not 0.6, else the noise is not continuous at simplex boundaries.
	double t = 0.5 - x * x - y * y - z * z - w * w;

	if (t < 0)
	{
		return 0.0;
	}

	t *= t;
	return t * t * Dot4(gradient, x, y, z, w);
}

// 2D simplex noise
double SimplexNoise2D(double xin, double yin) 
{
	// Skew the input space to determine which simplex cell we're in
	const double F2 = 0.36602540378; // 0.5 * (sqrt(3.0) - 1.0);
	double s = (xin + yin) * F2; // Hairy factor for 2D
	int i = FastFloor(xin + s);
	int j = FastFloor(yin + s);

	const double G2 = 0.2113248654; // (3.0 - sqrt(3.0)) / 6.0;
	const double t = (i + j) * G2;

	// Unskew the cell origin back to (x,y) space
	double X0 = i - t; 
	double Y0 = j - t;
	// The x,y distances from the cell origin
	double x0 = xin - X0;
	double y0 = yin - Y0;

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
	double x1 = x0 - i1 + G2; 
	double y1 = y0 - j1 + G2;
	// Offsets for last corner in (x,y) unskewed coords
	double x2 = x0 - 1.0 + 2.0 * G2; 
	double y2 = y0 - 1.0 + 2.0 * G2;

	// Work out the hashed gradient indices of the three simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int gi0 = g_HASH_TABLE_KEN_PERLIN[ii + g_HASH_TABLE_KEN_PERLIN[jj]] % 12;
	int gi1 = g_HASH_TABLE_KEN_PERLIN[ii + i1 + g_HASH_TABLE_KEN_PERLIN[jj + j1]] % 12;
	int gi2 = g_HASH_TABLE_KEN_PERLIN[ii + 1 + g_HASH_TABLE_KEN_PERLIN[jj + 1]] % 12;

	// Noise contributions from the three corners
	double n0, n1, n2;
	// Calculate the contribution from the three corners
	n0 = ContributionFromCorner3(grad4[gi0], x0, y0);
	n1 = ContributionFromCorner3(grad4[gi1], x1, y1);
	n2 = ContributionFromCorner3(grad4[gi2], x2, y2);

	// Add contributions from each corner to get the const noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 70.0 * (n0 + n1 + n2);
}

// 3D simplex noise
double SimplexNoise3D(double xin, double yin, double zin)
{
	// Skew the input space to determine which simplex cell we're in
	const double F3 = 1.0 / 3.0;
	// Very nice and simple skew factor for 3D
	double s = (xin + yin + zin) * F3;
	int i = FastFloor(xin + s);
	int j = FastFloor(yin + s);
	int k = FastFloor(zin + s);

	// Very nice and simple unskew factor, too
	const double G3 = 1.0 / 6.0; 
	const double t = (i + j + k) * G3;

	// Unskew the cell origin back to (x,y,z) space
	double X0 = i - t;
	double Y0 = j - t;
	double Z0 = k - t;

	// The x,y,z distances from the cell origin
	double x0 = xin - X0;
	double y0 = yin - Y0;
	double z0 = zin - Z0;

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
	double x1 = x0 - i1 + G3; 
	double y1 = y0 - j1 + G3;
	double z1 = z0 - k1 + G3;

	// Offsets for third corner in (x,y,z) coordinates
	double x2 = x0 - i2 + 2.0 * G3; 
	double y2 = y0 - j2 + 2.0 * G3;
	double z2 = z0 - k2 + 2.0 * G3;

	// Offsets for last corner in (x,y,z) coordinates
	double x3 = x0 - 1.0 + 3.0 * G3;
	double y3 = y0 - 1.0 + 3.0 * G3;
	double z3 = z0 - 1.0 + 3.0 * G3;

	// Work out the hashed gradient indices of the four simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;
	int gi0 = g_HASH_TABLE_KEN_PERLIN[ii + g_HASH_TABLE_KEN_PERLIN[jj + g_HASH_TABLE_KEN_PERLIN[kk]]] % 12;
	int gi1 = g_HASH_TABLE_KEN_PERLIN[ii + i1 + g_HASH_TABLE_KEN_PERLIN[jj + j1 + g_HASH_TABLE_KEN_PERLIN[kk + k1]]] % 12;
	int gi2 = g_HASH_TABLE_KEN_PERLIN[ii + i2 + g_HASH_TABLE_KEN_PERLIN[jj + j2 + g_HASH_TABLE_KEN_PERLIN[kk + k2]]] % 12;
	int gi3 = g_HASH_TABLE_KEN_PERLIN[ii + 1 + g_HASH_TABLE_KEN_PERLIN[jj + 1 + g_HASH_TABLE_KEN_PERLIN[kk + 1]]] % 12;

	// Noise contributions from the four corners
	double n0, n1, n2, n3;
	// Calculate the contribution from the four corners	
	n0 = ContributionFromCorner4(grad3[gi0], x0, y0, z0);
	n1 = ContributionFromCorner4(grad3[gi1], x1, y1, z1);
	n2 = ContributionFromCorner4(grad3[gi2], x2, y2, z2);
	n3 = ContributionFromCorner4(grad3[gi3], x3, y3, z3);

	// Add contributions from each corner to get the const noise value.
	// The result is scaled to stay just inside [-1,1]
	return 32.0 * (n0 + n1 + n2 + n3);
}

// 4D simplex noise
double SimplexNoise4D(double x, double y, double z, double w) {

	// The skewing and unskewing factors are hairy again for the 4D case
	const double F4 = 0.30901699437; // (sqrt(5.0) - 1.0) / 4.0;
	const double G4 = 0.13819660112; // (5.0 - sqrt(5.0)) / 20.0;	

	// Skew the (x,y,z,w) space to determine which cell of 24 simplicities we are in
	const double s = (x + y + z + w) * F4; // Factor for 4D skewing
	const int i = FastFloor(x + s);
	const int j = FastFloor(y + s);
	const int k = FastFloor(z + s);
	const int l = FastFloor(w + s);

	const double t = (i + j + k + l) * G4; // Factor for 4D unskewing
	const double X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
	const double Y0 = j - t;
	const double Z0 = k - t;
	const double W0 = l - t;

	const double x0 = x - X0; // The x,y,z,w distances from the cell origin
	const double y0 = y - Y0;
	const double z0 = z - Z0;
	const double w0 = w - W0;

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
	i1 = simplex[c][0] >= 3 ? 1 : 0;
	j1 = simplex[c][1] >= 3 ? 1 : 0;
	k1 = simplex[c][2] >= 3 ? 1 : 0;
	l1 = simplex[c][3] >= 3 ? 1 : 0;
	// The number 2 in the "simplex" array is at the second largest coordinate.
	i2 = simplex[c][0] >= 2 ? 1 : 0;
	j2 = simplex[c][1] >= 2 ? 1 : 0;
	k2 = simplex[c][2] >= 2 ? 1 : 0;
	l2 = simplex[c][3] >= 2 ? 1 : 0;
	// The number 1 in the "simplex" array is at the second smallest coordinate.
	i3 = simplex[c][0] >= 1 ? 1 : 0;
	j3 = simplex[c][1] >= 1 ? 1 : 0;
	k3 = simplex[c][2] >= 1 ? 1 : 0;
	l3 = simplex[c][3] >= 1 ? 1 : 0;

	// The fifth corner has all coordinate offsets = 1, so no need to look that up.
	// Offsets for second corner in (x,y,z,w) coordinates
	const double x1 = x0 - i1 + G4;
	const double y1 = y0 - j1 + G4;
	const double z1 = z0 - k1 + G4;
	const double w1 = w0 - l1 + G4;

	// Offsets for third corner in (x,y,z,w) coordinates
	const double x2 = x0 - i2 + 2.0 * G4;
	const double y2 = y0 - j2 + 2.0 * G4;
	const double z2 = z0 - k2 + 2.0 * G4;
	const double w2 = w0 - l2 + 2.0 * G4;

	// Offsets for fourth corner in (x,y,z,w) coordinates
	const double x3 = x0 - i3 + 3.0 * G4;
	const double y3 = y0 - j3 + 3.0 * G4;
	const double z3 = z0 - k3 + 3.0 * G4;
	const double w3 = w0 - l3 + 3.0 * G4;

	// Offsets for last corner in (x,y,z,w) coordinates
	const double x4 = x0 - 1.0 + 4.0 * G4;
	const double y4 = y0 - 1.0 + 4.0 * G4;
	const double z4 = z0 - 1.0 + 4.0 * G4;
	const double w4 = w0 - 1.0 + 4.0 * G4;

	// Work out the hashed gradient indices of the five simplex corners
	const int ii = i & 255;
	const int jj = j & 255;
	const int kk = k & 255;
	const int ll = l & 255;
	const int gi0 = g_HASH_TABLE_KEN_PERLIN[ii + g_HASH_TABLE_KEN_PERLIN[jj + g_HASH_TABLE_KEN_PERLIN[kk + g_HASH_TABLE_KEN_PERLIN[ll]]]] % 32;
	const int gi1 = g_HASH_TABLE_KEN_PERLIN[ii + i1 + g_HASH_TABLE_KEN_PERLIN[jj + j1 + g_HASH_TABLE_KEN_PERLIN[kk + k1 + g_HASH_TABLE_KEN_PERLIN[ll + l1]]]] % 32;
	const int gi2 = g_HASH_TABLE_KEN_PERLIN[ii + i2 + g_HASH_TABLE_KEN_PERLIN[jj + j2 + g_HASH_TABLE_KEN_PERLIN[kk + k2 + g_HASH_TABLE_KEN_PERLIN[ll + l2]]]] % 32;
	const int gi3 = g_HASH_TABLE_KEN_PERLIN[ii + i3 + g_HASH_TABLE_KEN_PERLIN[jj + j3 + g_HASH_TABLE_KEN_PERLIN[kk + k3 + g_HASH_TABLE_KEN_PERLIN[ll + l3]]]] % 32;
	const int gi4 = g_HASH_TABLE_KEN_PERLIN[ii + 1 + g_HASH_TABLE_KEN_PERLIN[jj + 1 + g_HASH_TABLE_KEN_PERLIN[kk + 1 + g_HASH_TABLE_KEN_PERLIN[ll + 1]]]] % 32;

	// Noise contributions from the five corners
	double n0, n1, n2, n3, n4; 
	// Calculate the contribution from the five corners
	n0 = ContributionFromCorner5(grad4[gi0], x0, y0, z0, w0);
	n1 = ContributionFromCorner5(grad4[gi1], x1, y1, z1, w1);
	n2 = ContributionFromCorner5(grad4[gi2], x2, y2, z2, w2);
	n3 = ContributionFromCorner5(grad4[gi3], x3, y3, z3, w3);
	n4 = ContributionFromCorner5(grad4[gi4], x4, y4, z4, w4);

	// Sum up and scale the result to cover the range [-1,1]
	return 27.0 * (n0 + n1 + n2 + n3 + n4);
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
	va_list list;
	va_start(list, nDim);
	for (int i = 0; i < nDim; i++)
	{
		input[i] = va_arg(list, double);
	}
	va_end(list);

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
	const int NUMBER_OF_GRADIENT_INDICES = 1 << nDim;                       // the number of gradient indices is = (2^number_of_dimensions)
	const int NUMBER_OF_EDGES = nDim * (1 << N_DIM_MINUS_ONE);              // the number of edges is = (number_of_dimensions * (2^(number_of_dimensions - 1)))

	// Arrays
	int* unitGridCells = new int[nDim];                                     // Find unit grid cell containing point
	int* gradientIndices = new int[NUMBER_OF_GRADIENT_INDICES];             // Calculate a set of eight hashed gradient indices
	int* grad3 = new int[NUMBER_OF_EDGES * nDim];                           // Calculate the gradient helper for noise contributions
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

	// Calculate the gradient helper for noise contributions
	// make an exception for nDim = 1;
	switch (nDim)
	{
	case 1:
		grad3[0] = 0;
		break;
	case 2:
		break;
	default:
		for (int i = 0; i < NUMBER_OF_EDGES; i++)
		{
			int k = 0;
			int perm = i / N_DIM_PLUS_ONE;
			for (int j = 0; j < nDim; j++)
			{
				grad3[i * nDim + j] = perm == j
					? 0
					: CheckBitStatus(i, nDim - 2 - (k++)) == 0 ? 1 : -1;
			}
		}
		break;
	}

	// Calculate noise contributions from each corner
	for (int i = 0; i < NUMBER_OF_GRADIENT_INDICES; i++)
	{
		// Here we do a DOT PRODUCT for each corner
		noiseContributions[i] = 0;
		for (int j = 0; j < nDim; j++)
		{
			int scalar = CheckBitStatus(i, N_DIM_MINUS_ONE - j) ? -1 : 0;
			noiseContributions[i] += (input[j] + scalar) * grad3[gradientIndices[i] * nDim + j];
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
	delete[] grad3;
	delete[] noiseContributions;

	return result;
}