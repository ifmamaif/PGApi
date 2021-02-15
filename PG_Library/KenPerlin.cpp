#include "pch.h"
#include "KenPerlin.h"

#include "Math.h"

#include <math.h>
#include <cstdarg>

// Hash lookup table as defined by Ken Perlin.  This is a randomly
// arranged array of all numbers from 0-255 inclusive.
// Doubled permutation to avoid overflow
static const int permutation[] =
{
	// first 256 values
	151,	160,	137,	91,		90,		15,		131,	13,		201,	95,		96,		53,		194,	233,	7,		225,
	140,	36,		103,	30,		69,		142,	8,		99,		37,		240,	21,		10,		23,		190,	6,		148,
	247,	120,	234,	75,		0,		26,		197,	62,		94,		252,	219,	203,	117,	35,		11,		32,
	57,		177,	33,		88,		237,	149,	56,		87,		174,	20,		125,	136,	171,	168,	68,		175,
	74,		165,	71,		134,	139,	48,		27,		166,	77,		146,	158,	231,	83,		111,	229,	122,
	60,		211,	133,	230,	220,	105,	92,		41,		55,		46,		245,	40,		244,	102,	143,	54,
	65,		25,		63,		161,	1,		216,	80,		73,		209,	76,		132,	187,	208,	89,		18,		169,
	200,	196,	135,	130,	116,	188,	159,	86,		164,	100,	109,	198,	173,	186,	3,		64,
	52,		217,	226,	250,	124,	123,	5,		202,	38,		147,	118,	126,	255,	82,		85,		212,
	207,	206,	59,		227,	47,		16,		58,		17,		182,	189,	28,		42,		223,	183,	170,	213,
	119,	248,	152,	2,		44,		154,	163,	70,		221,	153,	101,	155,	167,	43,		172,	9,
	129,	22,		39,		253,	19,		98,		108,	110,	79,		113,	224,	232,	178,	185,	112,	104,
	218,	246,	97,		228,	251,	34,		242,	193,	238,	210,	144,	12,		191,	179,	162,	241,
	81,		51,		145,	235,	249,	14,		239,	107,	49,		192,	214,	31,		181,	199,	106,	157,
	184,	84,		204,	176,	115,	121,	50,		45,		127,	4,		150,	254,	138,	236,	205,	93,
	222,	114,	67,		29,		24,		72,		243,	141,	128,	195,	78,		66,		215,	61,		156,	180,
   // To remove the need for index wrapping, double the permutation table length
   // duplicate 256 values
	151,	160,	137,	91,		90,		15,		131,	13,		201,	95,		96,		53,		194,	233,	7,		225,
	140,	36,		103,	30,		69,		142,	8,		99,		37,		240,	21,		10,		23,		190,	6,		148,
	247,	120,	234,	75,		0,		26,		197,	62,		94,		252,	219,	203,	117,	35,		11,		32,
	57,		177,	33,		88,		237,	149,	56,		87,		174,	20,		125,	136,	171,	168,	68,		175,
	74,		165,	71,		134,	139,	48,		27,		166,	77,		146,	158,	231,	83,		111,	229,	122,
	60,		211,	133,	230,	220,	105,	92,		41,		55,		46,		245,	40,		244,	102,	143,	54,
	65,		25,		63,		161,	1,		216,	80,		73,		209,	76,		132,	187,	208,	89,		18,		169,
	200,	196,	135,	130,	116,	188,	159,	86,		164,	100,	109,	198,	173,	186,	3,		64,
	52,		217,	226,	250,	124,	123,	5,		202,	38,		147,	118,	126,	255,	82,		85,		212,
	207,	206,	59,		227,	47,		16,		58,		17,		182,	189,	28,		42,		223,	183,	170,	213,
	119,	248,	152,	2,		44,		154,	163,	70,		221,	153,	101,	155,	167,	43,		172,	9,
	129,	22,		39,		253,	19,		98,		108,	110,	79,		113,	224,	232,	178,	185,	112,	104,
	218,	246,	97,		228,	251,	34,		242,	193,	238,	210,	144,	12,		191,	179,	162,	241,
	81,		51,		145,	235,	249,	14,		239,	107,	49,		192,	214,	31,		181,	199,	106,	157,
	184,	84,		204,	176,	115,	121,	50,		45,		127,	4,		150,	254,	138,	236,	205,	93,
	222,	114,	67,		29,		24,		72,		243,	141,	128,	195,	78,		66,		215,	61,		156,	180,
};

static const int grad3[12][3] =
//{
//	{1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0},
//	{1,0,1},{-1,0,1},{1,0,-1},{-1,0,-1},
//	{0,1,1},{0,-1,1},{0,1,-1},{0,-1,-1}
//};
{
	{ 0,1,1}, { 0,1,-1 }, {0,-1,1 }, { 0,-1,-1 },
	{ 1,0,1 }, { 1,0,-1 }, { -1,0,1 }, { -1,0,-1 },
	{ 1,1,0 }, { 1,-1,0 }, { -1,1,0 }, { -1,-1,0 }
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
		total += PerlinNoise3D(x * frequency, y * frequency, z * frequency) * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		frequency *= 2;
	}

	return total / maxValue;
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
	int gi000 = permutation[xi + permutation[yi + permutation[zi]]] % 12;
	int gi001 = permutation[xi + permutation[yi + permutation[zi + 1]]] % 12;
	int gi010 = permutation[xi + permutation[yi + 1 + permutation[zi]]] % 12;
	int gi011 = permutation[xi + permutation[yi + 1 + permutation[zi + 1]]] % 12;
	int gi100 = permutation[xi + 1 + permutation[yi + permutation[zi]]] % 12;
	int gi101 = permutation[xi + 1 + permutation[yi + permutation[zi + 1]]] % 12;
	int gi110 = permutation[xi + 1 + permutation[yi + 1 + permutation[zi]]] % 12;
	int gi111 = permutation[xi + 1 + permutation[yi + 1 + permutation[zi + 1]]] % 12;

	// Calculate noise contributions from each of the eight corners
	double n000 = Dot3(grad3[gi000], x, y, z);
	double n001 = Dot3(grad3[gi001], x, y, z - 1);
	double n010 = Dot3(grad3[gi010], x, y - 1, z);
	double n011 = Dot3(grad3[gi011], x, y - 1, z - 1);
	double n100 = Dot3(grad3[gi100], x - 1, y, z);
	double n101 = Dot3(grad3[gi101], x - 1, y, z - 1);
	double n110 = Dot3(grad3[gi110], x - 1, y - 1, z);
	double n111 = Dot3(grad3[gi111], x - 1, y - 1, z - 1);

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


double PerlinNoise3D(double x, double y, double z)
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

	int aaa = permutation[permutation[permutation[xi] + yi] + zi];
	int aba = permutation[permutation[permutation[xi] + yi + 1] + zi];
	int aab = permutation[permutation[permutation[xi] + yi] + zi + 1];
	int abb = permutation[permutation[permutation[xi] + yi + 1] + zi + 1];
	int baa = permutation[permutation[permutation[xi + 1] + yi] + zi];
	int bba = permutation[permutation[permutation[xi + 1] + yi + 1] + zi];
	int bab = permutation[permutation[permutation[xi + 1] + yi] + zi + 1];
	int bbb = permutation[permutation[permutation[xi + 1] + yi + 1] + zi + 1];

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

double PerlinNoise2D(double x, double y)
{
	const int xi = (int)x & 255;
	const int yi = (int)y & 255;
	const double xf = x - (int)x;
	const double yf = y - (int)y;
	const double u = Fade(xf);
	const double v = Fade(yf);

	const int aaa = permutation[permutation[permutation[xi] + yi]];
	const int aba = permutation[permutation[permutation[xi] + yi + 1]];
	const int baa = permutation[permutation[permutation[xi + 1] + yi]];
	const int bba = permutation[permutation[permutation[xi + 1] + yi + 1]];

	double x1, x2;
	x1 = Lerpd(Gradient(aaa, xf, yf, 0),
		Gradient(baa, xf - 1, yf, 0),
		u);
	x2 = Lerpd(Gradient(aba, xf, yf - 1, 0),
		Gradient(bba, xf - 1, yf - 1, 0),
		u);

	return MutationValue(Lerpd(x1, x2, v));
}

double PerlinNoise(double x)
{
	const int xi = (int)x & 255;
	const double xf = x - (int)x;

	const int aaa = permutation[permutation[permutation[xi]]];
	const int baa = permutation[permutation[permutation[xi + 1]]];

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
			noiseMap[x][y] = (double)PerlinNoise3D(xCoord, yCoord, 0.f);
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
// • Simplex noise has a lower computational complexity and requires fewer multiplications.
// • Simplex noise scales to higher dimensions(4D, 5D and up) with much less computational
//		cost, the complexity is for dimensions instead of the of classic Noise.
// • Simplex noise has no noticeable directional artifacts.
// • Simplex noise has a well - defined and continuous gradient everywhere that can be computed
//		quite cheaply.
// • Simplex noise is easy to implement in hardware.
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
	int gi0 = permutation[ii + permutation[jj]] % 12;
	int gi1 = permutation[ii + i1 + permutation[jj + j1]] % 12;
	int gi2 = permutation[ii + 1 + permutation[jj + 1]] % 12;

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
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		} // X Y Z order
		else if (x0 >= z0)
		{
			i1 = 1; 
			j1 = 0;
			k1 = 0; 
			i2 = 1;
			j2 = 0; 
			k2 = 1;
		} // X Z Y order
		else 
		{
			i1 = 0;
			j1 = 0;
			k1 = 1;
			i2 = 1;
			j2 = 0;
			k2 = 1;
		} // Z X Y order
	}
	else
	{ // x0<y0
		if (y0 < z0)
		{
			i1 = 0;
			j1 = 0;
			k1 = 1;
			i2 = 0;
			j2 = 1;
			k2 = 1;
		} // Z Y X order
		else if (x0 < z0)
		{
			i1 = 0;
			j1 = 1;
			k1 = 0;
			i2 = 0;
			j2 = 1;
			k2 = 1;
		} // Y Z X order
		else
		{
			i1 = 0;
			j1 = 1;
			k1 = 0;
			i2 = 1;
			j2 = 1;
			k2 = 0;
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
	int gi0 = permutation[ii + permutation[jj + permutation[kk]]] % 12;
	int gi1 = permutation[ii + i1 + permutation[jj + j1 + permutation[kk + k1]]] % 12;
	int gi2 = permutation[ii + i2 + permutation[jj + j2 + permutation[kk + k2]]] % 12;
	int gi3 = permutation[ii + 1 + permutation[jj + 1 + permutation[kk + 1]]] % 12;

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
	const int gi0 = permutation[ii + permutation[jj + permutation[kk + permutation[ll]]]] % 32;
	const int gi1 = permutation[ii + i1 + permutation[jj + j1 + permutation[kk + k1 + permutation[ll + l1]]]] % 32;
	const int gi2 = permutation[ii + i2 + permutation[jj + j2 + permutation[kk + k2 + permutation[ll + l2]]]] % 32;
	const int gi3 = permutation[ii + i3 + permutation[jj + j3 + permutation[kk + k3 + permutation[ll + l3]]]] % 32;
	const int gi4 = permutation[ii + 1 + permutation[jj + 1 + permutation[kk + 1 + permutation[ll + 1]]]] % 32;

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

	// Get the input
	double* input = new double[nDim];
	va_list list;
	va_start(list, nDim);
	for (int i = 0; i < nDim; i++)
	{
		input[i] = va_arg(list, double);
	}
	va_end(list);
	//return input;

	// Find unit grid cell containing point
	int* unitGridCells = new int[nDim];
	for (int i = 0; i < nDim; i++)
	{
		unitGridCells[i] = FastFloor(input[i]);
	}

	// Get relative coordinates of point within that cell
	for (int i = 0; i < nDim; i++)
	{
		input[i] = input[i] - unitGridCells[i];
	}

	// Wrap the integer cells at 255 (smaller integer period can be introduced here)
	for (int i = 0; i < nDim; i++)
	{
		unitGridCells[i] &= 255;
	}

	// Calculate a set of eight hashed gradient indices
	int numberOfGradientIndices = 1 << nDim;
	int* gradientIndices = new int[numberOfGradientIndices];
	int numberOfEdges = (int)(nDim * (1 << (nDim - 1)));
	for (int i = 0; i < numberOfGradientIndices; i++)
	{
		gradientIndices[i] = 0;
		for (int dim = 0; dim < nDim; dim++)
		{
			gradientIndices[i] = permutation[unitGridCells[2 - dim] +
				CheckBitStatus(i, dim) +
				gradientIndices[i]];
	
		}
		gradientIndices[i] %= numberOfEdges;
	}

	// Calculate noise contributions from each of the eight corners
	double* noiseContributions = new double[numberOfGradientIndices];
	
	int* grad3 = new int[numberOfEdges * nDim];
	for (int i = 0; i < numberOfEdges; i++)
	{
		int* grad1 = new int[nDim - 1];
		for (int j = 0; j < (nDim - 1); j++)
		{
			grad1[j] = CheckBitStatus(i, nDim - 2 - j) == 0 ? 1 : -1;
		}

		int k = 0;
		int perm = i / (nDim + 1);
		int* grad2 = new int[nDim];
		for (int j = 0; j < nDim; j++)
		{
			grad2[j] = perm == j ? 0 : grad1[k++];
		}
	
		for (int j = 0; j < nDim; j++)
		{
			grad3[i * 3 + j] = grad2[j];
		}
	}
	
	int* scalar = new int[numberOfEdges * nDim];
	for (int i = 0; i < numberOfEdges; i++)
	{
		for (int j = 0; j < nDim; j++)
		{
			scalar[i * 3 + j] = CheckBitStatus(i, nDim - 1 - j) ? -1 : 0;
		}
	}
	
	for (int i = 0; i < numberOfGradientIndices; i++)
	{
		int input0 = input[0];
		int input1 = input[1];
		int input2 = input[2];
		int scalar0 = scalar[i * 3 + 0];
		int scalar1 = scalar[i * 3 + 1];
		int scalar2 = scalar[i * 3 + 2];
		int gradientIndice = gradientIndices[i];
		int grad0 = grad3[gradientIndice * 3 + 0];
		int grad1 = grad3[gradientIndice * 3 + 1];
		int grad2 = grad3[gradientIndice * 3 + 2];
	
		noiseContributions[i] = (input[0] + scalar[i * 3 + 0]) * grad3[gradientIndices[i] * 3 + 0] +
			(input[1] + scalar[i * 3 + 1]) * grad3[gradientIndices[i] * 3 + 1] +
			(input[2] + scalar[i * 3 + 2]) * grad3[gradientIndices[i] * 3 + 2];
	}

	// Compute the fade curve value for each of x, y, z
	double* curves = new double[nDim];
	for (int i = 0; i < nDim; i++)
	{
		curves[i] = Fade(input[i]);
	}

	// Interpolate along x the contributions from each of the corners
	int interpolations = numberOfGradientIndices / 2;
	for (int i = 0; i < nDim; i++)
	{
		for (int j = 0; j < interpolations; j++)
		{
			noiseContributions[j] = Lerpd(noiseContributions[j], noiseContributions[j + interpolations], curves[i]);
		}
		interpolations /= 2;
	}
	return noiseContributions[0];
}