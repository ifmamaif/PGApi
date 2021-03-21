#include "pch.h"
#include "PerlinNoiseImproved.h"

#include "Math.h"
#include "Constants.h"
#include "Allocation.h"
#include "PerlinUtils.h"

double MutationValue(double x)
{
	//return (x + 1) / 2;					     	// For convenience we bound it to 0 - 1 (theoretical min/max before is -1 - 1)
	return (x + 0.7) / 1.5;                         // Magic value to obtain [-0.2,1.13] 
}

//IMPLEMENTATION OF IMPROVED NOISE - COPYRIGHT 2002 KEN PERLIN.
//This code implements the algorithm as described in a corresponding SIGGRAPH 2002 paper.
double PerlinNoise_Improved3D(double x, double y, double z)
{
	// Integer part of input
	int xi = FloorToIntd(x),
		yi = FloorToIntd(y),
		zi = FloorToIntd(z);

	// FIND RELATIVE X,Y,Z OF POINT IN CUBE.
	// Fractional part of input
	x -= xi;
	y -= yi;
	z -= zi;

	// FIND UNIT CUBE THAT CONTAINS POINT.
	// Wrap to 0..255
	xi = xi & 255;
	yi = yi & 255;
	zi = zi & 255;

	// HASH COORDINATES OF THE 8 CUBE CORNERS,
	int a = g_HASH_TABLE_KEN_PERLIN[xi];
	int b = g_HASH_TABLE_KEN_PERLIN[xi + 1];
	int	aa = g_HASH_TABLE_KEN_PERLIN[a + yi];		// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi		]
	int	ab = g_HASH_TABLE_KEN_PERLIN[a + yi + 1];	// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi + 1 ]
	int	ba = g_HASH_TABLE_KEN_PERLIN[b + yi];		// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi		]
	int	bb = g_HASH_TABLE_KEN_PERLIN[b + yi + 1];	// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi + 1 ]

	int aaa = g_HASH_TABLE_KEN_PERLIN[aa + zi];		// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi		] + zi	]
	int baa = g_HASH_TABLE_KEN_PERLIN[ba + zi];		// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi		] + zi	]
	int aba = g_HASH_TABLE_KEN_PERLIN[ab + zi];		// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi + 1 ] + zi	]
	int bba = g_HASH_TABLE_KEN_PERLIN[bb + zi];		// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi + 1 ] + zi	]
	int aab = g_HASH_TABLE_KEN_PERLIN[aa + zi + 1]; // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi		] + zi+1]
	int abb = g_HASH_TABLE_KEN_PERLIN[ab + zi + 1]; // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi		] + zi+1]
	int bab = g_HASH_TABLE_KEN_PERLIN[ba + zi + 1]; // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi + 1	] + zi+1]
	int bbb = g_HASH_TABLE_KEN_PERLIN[bb + zi + 1]; // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi + 1	] + zi+1]

	// Calculate gradients of the cube 8 Corners
	double g000 = GradientUnity(aaa, x, y, z);
	double g001 = GradientUnity(baa, x - 1, y, z);
	double g002 = GradientUnity(aba, x, y - 1, z);
	double g003 = GradientUnity(bba, x - 1, y - 1, z);
	double g004 = GradientUnity(aab, x, y, z - 1);
	double g005 = GradientUnity(bab, x - 1, y, z - 1);
	double g006 = GradientUnity(abb, x, y - 1, z - 1);
	double g007 = GradientUnity(bbb, x - 1, y - 1, z - 1);

	// COMPUTE FADE CURVES FOR EACH OF X,Y,Z.
	double u = Fade(x),
		v = Fade(y),
		w = Fade(z);

	// AND ADD BLENDED RESULTS FROM  8 CORNERS OF CUBE
	// Interpolations
	// first
	double l01 = Lerpd(g000, g001, u);
	double l02 = Lerpd(g002, g003, u);
	double l03 = Lerpd(g004, g005, u);
	double l04 = Lerpd(g006, g007, u);

	// second
	double l11 = Lerpd(l01, l02, v);
	double l12 = Lerpd(l03, l04, v);

	// third
	double l20 = Lerpd(l11, l12, w);

	return l20;
}

double PerlinNoise_Improved2D(double x, double y)
{
	// Integer part of input
	int xi = FloorToIntd(x),
		yi = FloorToIntd(y);

	// FIND RELATIVE X,Y,Z OF POINT IN CUBE.
	// Fractional part of input
	x -= xi;
	y -= yi;

	// FIND UNIT CUBE THAT CONTAINS POINT.
	// Wrap to 0..255
	xi = xi & 255;
	yi = yi & 255;

	// HASH COORDINATES OF THE 8 CUBE CORNERS,
	int A = g_HASH_TABLE_KEN_PERLIN[xi] + yi;
	int B = g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi;
	int	AA = g_HASH_TABLE_KEN_PERLIN[A];
	int	AB = g_HASH_TABLE_KEN_PERLIN[A + 1];
	int	BA = g_HASH_TABLE_KEN_PERLIN[B];
	int	BB = g_HASH_TABLE_KEN_PERLIN[B + 1];

	// Calculate gradients of the cube 8 Corners
	double g000 = GradientUnity(g_HASH_TABLE_KEN_PERLIN[AA], x, y, 0);			// bottom left
	double g001 = GradientUnity(g_HASH_TABLE_KEN_PERLIN[BA], x - 1, y, 0);		// bottom right
	double g002 = GradientUnity(g_HASH_TABLE_KEN_PERLIN[AB], x, y - 1, 0);		// top left
	double g003 = GradientUnity(g_HASH_TABLE_KEN_PERLIN[BB], x - 1, y - 1, 0);	// top right

	// COMPUTE FADE CURVES FOR EACH OF X,Y,Z.
	double u = Fade(x),
		v = Fade(y);

	// AND ADD BLENDED RESULTS FROM  8 CORNERS OF CUBE
	// Interpolations
	// first
	double l01 = Lerpd(g000, g001, u);
	double l02 = Lerpd(g002, g003, u);

	// second
	double l11 = Lerpd(l01, l02, v);

	return l11;
}

double PerlinNoise_Improved1D(double x)
{
	// Integer part of input
	int xi = FloorToIntd(x);
	
	// FIND RELATIVE X,Y,Z OF POINT IN CUBE.
	// Fractional part of input
	x -= xi;

	// FIND UNIT CUBE THAT CONTAINS POINT.
	// Wrap to 0..255
	xi = xi & 255;

	// HASH COORDINATES OF THE 8 CUBE CORNERS
	int	AA = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi]];
	int	BA = g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1]];

	// Calculate gradients of the cube 8 Corners
	double g000 = GradientUnity(g_HASH_TABLE_KEN_PERLIN[AA], x, 0, 0);
	double g001 = GradientUnity(g_HASH_TABLE_KEN_PERLIN[BA], x - 1, 0, 0);

	// COMPUTE FADE CURVES FOR EACH OF X,Y,Z.
	double u = Fade(x);

	// AND ADD BLENDED RESULTS FROM  8 CORNERS OF CUBE
	// Interpolations
	double l01 = Lerpd(g000, g001, u);

	return l01;
}