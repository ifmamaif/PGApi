#include "pch.h"
#include "SimplexNoise.h"

#include "Math.h"
#include "Constants.h"
#include "Allocation.h"
#include "PerlinUtils.h"

#include <tuple>
#include <iostream>

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

static int fastfloor(double x) {
	return x > 0 ? (int)x : (int)x - 1;
}
static float dot(const int g[], float x, float y) {
	return g[0] * x + g[1] * y;
}
static float dot(const int g[], float x, float y, float z) {
	return g[0] * x + g[1] * y + g[2] * z;
}
static float dot(const int g[], float x, float y, float z, float w) {
	return g[0] * x + g[1] * y + g[2] * z + g[3] * w;
}

template <class... Args>
float ContributionFromCorners(const int gradient[],const int nInput , Args&&... args)
{
	float* input = new float[nInput];

	auto arguments = std::make_tuple(std::forward<Args>(args)...);
	for (int i = 0; i < nInput; i++)
	{
		input[i] = std::get<0>(arguments);
	}
	auto da = std::get<0>(arguments);
	auto da1 = std::get<1>(arguments);

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
	float result = t * t * ProdusScalarVector(nInput, gradient, input);
	delete[] input;
	return result;
}

float ContributiaColtului2D(int gi, float x, float y)
{
	float t0 = 0.5f - x * x - y * y;
	if (t0 < 0)
		return 0.0f;

	t0 *= t0;
	return t0 * t0 * dot(g_GRAD3_Simplex[gi], x, y); // (x,y) of grad3 used for 2D gradient
}

float Contribution3D(int gi, float x, float y, float z)
{
	float t0 = 0.5f - x * x - y * y - z * z;
	if (t0 < 0)
		return 0.0f;

	t0 *= t0;
	return t0 * t0 * dot(g_GRAD3_Simplex[gi], x, y, z); // (x,y) of grad3 used for 2D gradient
}

float Contribution4D(int gi, float x, float y, float z, float w)
{
	float t0 = 0.5f - x * x - y * y - z * z - w * w;
	if (t0 < 0)
		return 0.0f;

	t0 *= t0;
	return t0 * t0 * dot(grad4[gi], x, y, z, w); // (x,y) of grad3 used for 2D gradient
}

float ZgomotulPerlin_Simplex1D(float x) {
	float n0, n1; 

	int i0 = ParteaÎntreagăRapidf(x);
	int i1 = i0 + 1;
	
	float x0 = x - i0;
	float x1 = x0 - 1.0f;

	float t0 = 1.0f - x0 * x0;
	t0 *= t0;
	n0 = t0 * t0 * Gradient1D(g_TABELA_HASH_KEN_PERLIN[i0], x0);

	float t1 = 1.0f - x1 * x1;
	t1 *= t1;
	n1 = t1 * t1 * Gradient1D(g_TABELA_HASH_KEN_PERLIN[i1], x1);

	return 0.395f * (n0 + n1);
}

float ZgomotulPerlin_Simplex2D(float x, float y)
{
	/// I. Aflarea punctelor simplexului (triunghului echilateral în 2D)
	const float FACTOR_ÎNCLINARE = 0.36602540378f;	// 0.5 * (sqrt(3.0) - 1.0);
	float valoareÎnclinare = (x + y) * FACTOR_ÎNCLINARE;
	int xÎntreg = ParteaÎntreagăRapidf(x + valoareÎnclinare),
		yÎntreg = ParteaÎntreagăRapidf(y + valoareÎnclinare);

	const float FACTOR_CORECȚIE = 0.2113248654f;	// (3.0 - sqrt(3.0)) / 6.0;
	const float valoareCorecție = (xÎntreg + yÎntreg) * FACTOR_CORECȚIE;
	float x1 = xÎntreg - valoareCorecție,
		y1 = yÎntreg - valoareCorecție;

	// Punctul 1, 
	// distanță de la punctul original
	x1 = x - x1;
	y1 = y - y1;

	// Determinarae decalajului
	int decalajX, decalajY;
	if (x1 > y1) // triunghiul de jos: (0,0)->(1,0)->(1,1)
	{
		decalajX = 1;
		decalajY = 0;
	}
	else		// triunghiul de sus: (0,0)->(0,1)->(1,1)
	{
		decalajX = 0;
		decalajY = 1;
	}

	// Punctul 2
	float x2 = x1 - decalajX + FACTOR_CORECȚIE,
		y2 = y1 - decalajY + FACTOR_CORECȚIE,
	// Punctul 3
		x3 = x1 - 1.0f + 2.0f * FACTOR_CORECȚIE,
		y3 = y1 - 1.0f + 2.0f * FACTOR_CORECȚIE;

	/// II. Calcularea punctelor de gradient
	int indiceGradientX = xÎntreg & 255,
		indiceGradientY = yÎntreg & 255;
	int gradient1 = g_TABELA_HASH_KEN_PERLIN[indiceGradientX + g_TABELA_HASH_KEN_PERLIN[indiceGradientY]] % 12;
	int gradient2 = g_TABELA_HASH_KEN_PERLIN[indiceGradientX + decalajX + g_TABELA_HASH_KEN_PERLIN[indiceGradientY + decalajY]] % 12;
	int gradient3 = g_TABELA_HASH_KEN_PERLIN[indiceGradientX + 1 + g_TABELA_HASH_KEN_PERLIN[indiceGradientY + 1]] % 12;

	/// III. Calcularea contribuțiilor de zgomot din cele trei colțuri
	float contributieZgomot1 = ContributiaColtului2D(gradient1, x1, y1),
		contributieZgomot2 = ContributiaColtului2D(gradient2, x2, y2),
		contributieZgomot3 = ContributiaColtului2D(gradient3, x3, y3);

	// Însumarea contribuțiilor pentru a obține valoarea zgomotului
	// Rezultatul este scalat pentru a returna valori din interval [-1,1]
	return 70.0f * (contributieZgomot1 + contributieZgomot2 + contributieZgomot3);
}

// 3D simplex noise
float ZgomotulPerlin_Simplex3D(float xin, float yin, float zin)
{
	const float F3 = 0.33333333333f; // 1.0f / 3.0f;
	// Very nice and simple skew factor for 3D
	float s = (xin + yin + zin) * F3;
	int i = ParteaÎntreagăRapidf(xin + s);
	int j = ParteaÎntreagăRapidf(yin + s);
	int k = ParteaÎntreagăRapidf(zin + s);

	const float G3 = 0.16666666666f; 
	const float t = (i + j + k) * G3;

	float x0 = i - t;
	float y0 = j - t;
	float z0 = k - t;

	x0 = xin - x0;
	y0 = yin - y0;
	z0 = zin - z0;

	int i1, j1, k1;
	int i2, j2, k2;

	if (x0 >= y0)
	{
		if (y0 >= z0)			// x0 >= y0 >= z0
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		} 
		else if (x0 >= z0)		// x0 >= y0, z0
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1;
		} 
		else // x0 < z0			// x0 >= y0, x0<z0
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1;
		} 
	}
	else
	{ // x0<y0
		if (y0 < z0)			// x0 < y0 < z0
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1;
		} 
		else if (x0 < z0)		// x0 < y0,z0
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;
		} 
		else					// x0<y0, x0>=z0
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		} 
	}

	float x1 = x0 - i1 + G3;
	float y1 = y0 - j1 + G3;
	float z1 = z0 - k1 + G3;

	float x2 = x0 - i2 + 2.0f * G3;
	float y2 = y0 - j2 + 2.0f * G3;
	float z2 = z0 - k2 + 2.0f * G3;

	float x3 = x0 - 1.0f + 3.0f * G3;
	float y3 = y0 - 1.0f + 3.0f * G3;
	float z3 = z0 - 1.0f + 3.0f * G3;

	i = i & 255;
	j = j & 255;
	k = k & 255;
	int gi0 = g_TABELA_HASH_KEN_PERLIN[i + g_TABELA_HASH_KEN_PERLIN[j + g_TABELA_HASH_KEN_PERLIN[k]]] % 12;
	int gi1 = g_TABELA_HASH_KEN_PERLIN[i + i1 + g_TABELA_HASH_KEN_PERLIN[j + j1 + g_TABELA_HASH_KEN_PERLIN[k + k1]]] % 12;
	int gi2 = g_TABELA_HASH_KEN_PERLIN[i + i2 + g_TABELA_HASH_KEN_PERLIN[j + j2 + g_TABELA_HASH_KEN_PERLIN[k + k2]]] % 12;
	int gi3 = g_TABELA_HASH_KEN_PERLIN[i + 1 + g_TABELA_HASH_KEN_PERLIN[j + 1 + g_TABELA_HASH_KEN_PERLIN[k + 1]]] % 12;

	float n0, n1, n2, n3;	
	n0 = Contribution3D(gi0, x0, y0, z0);
	n1 = Contribution3D(gi1, x1, y1, z1);
	n2 = Contribution3D(gi2, x2, y2, z2);
	n3 = Contribution3D(gi3, x3, y3, z3);

	return 32.0f * (n0 + n1 + n2 + n3);
}

float ZgomotulPerlin_Simplex4D(float x, float y, float z, float w) {

	const float F4 = 0.30901699437f;
	const float G4 = 0.13819660112f;

	const float s = (x + y + z + w) * F4;
	int i = ParteaÎntreagăRapidf(x + s);
	int j = ParteaÎntreagăRapidf(y + s);
	int k = ParteaÎntreagăRapidf(z + s);
	int l = ParteaÎntreagăRapidf(w + s);

	const float t = (i + j + k + l) * G4; 
	float x0 = i - t;
	float y0 = j - t;
	float z0 = k - t;
	float w0 = l - t;

	x0 = x - x0;
	y0 = y - y0;
	z0 = z - z0;
	w0 = w - w0;

	const int c1 = (x0 > y0) ? 32 : 0;
	const int c2 = (x0 > z0) ? 16 : 0;
	const int c3 = (y0 > z0) ? 8 : 0;
	const int c4 = (x0 > w0) ? 4 : 0;
	const int c5 = (y0 > w0) ? 2 : 0;
	const int c6 = (z0 > w0) ? 1 : 0;
	const int c = c1 + c2 + c3 + c4 + c5 + c6;

	int i1, j1, k1, l1;
	int i2, j2, k2, l2;
	int i3, j3, k3, l3;

	i1 = SIMPLEX_TABLE[c][0] >= 3 ? 1 : 0;
	j1 = SIMPLEX_TABLE[c][1] >= 3 ? 1 : 0;
	k1 = SIMPLEX_TABLE[c][2] >= 3 ? 1 : 0;
	l1 = SIMPLEX_TABLE[c][3] >= 3 ? 1 : 0;

	i2 = SIMPLEX_TABLE[c][0] >= 2 ? 1 : 0;
	j2 = SIMPLEX_TABLE[c][1] >= 2 ? 1 : 0;
	k2 = SIMPLEX_TABLE[c][2] >= 2 ? 1 : 0;
	l2 = SIMPLEX_TABLE[c][3] >= 2 ? 1 : 0;

	i3 = SIMPLEX_TABLE[c][0] >= 1 ? 1 : 0;
	j3 = SIMPLEX_TABLE[c][1] >= 1 ? 1 : 0;
	k3 = SIMPLEX_TABLE[c][2] >= 1 ? 1 : 0;
	l3 = SIMPLEX_TABLE[c][3] >= 1 ? 1 : 0;

	const float x1 = x0 - i1 + G4;
	const float y1 = y0 - j1 + G4;
	const float z1 = z0 - k1 + G4;
	const float w1 = w0 - l1 + G4;

	const float x2 = x0 - i2 + 2.0f * G4;
	const float y2 = y0 - j2 + 2.0f * G4;
	const float z2 = z0 - k2 + 2.0f * G4;
	const float w2 = w0 - l2 + 2.0f * G4;

	const float x3 = x0 - i3 + 3.0f * G4;
	const float y3 = y0 - j3 + 3.0f * G4;
	const float z3 = z0 - k3 + 3.0f * G4;
	const float w3 = w0 - l3 + 3.0f * G4;

	const float x4 = x0 - 1.0f + 4.0f * G4;
	const float y4 = y0 - 1.0f + 4.0f * G4;
	const float z4 = z0 - 1.0f + 4.0f * G4;
	const float w4 = w0 - 1.0f + 4.0f * G4;

	i = i & 255;
	j = j & 255;
	k = k & 255;
	l = l & 255;
	const int gi0 = g_TABELA_HASH_KEN_PERLIN[i + g_TABELA_HASH_KEN_PERLIN[j + g_TABELA_HASH_KEN_PERLIN[k + g_TABELA_HASH_KEN_PERLIN[l]]]] % 32;
	const int gi1 = g_TABELA_HASH_KEN_PERLIN[i + i1 + g_TABELA_HASH_KEN_PERLIN[j + j1 + g_TABELA_HASH_KEN_PERLIN[k + k1 + g_TABELA_HASH_KEN_PERLIN[l + l1]]]] % 32;
	const int gi2 = g_TABELA_HASH_KEN_PERLIN[i + i2 + g_TABELA_HASH_KEN_PERLIN[j + j2 + g_TABELA_HASH_KEN_PERLIN[k + k2 + g_TABELA_HASH_KEN_PERLIN[l + l2]]]] % 32;
	const int gi3 = g_TABELA_HASH_KEN_PERLIN[i + i3 + g_TABELA_HASH_KEN_PERLIN[j + j3 + g_TABELA_HASH_KEN_PERLIN[k + k3 + g_TABELA_HASH_KEN_PERLIN[l + l3]]]] % 32;
	const int gi4 = g_TABELA_HASH_KEN_PERLIN[i + 1 + g_TABELA_HASH_KEN_PERLIN[j + 1 + g_TABELA_HASH_KEN_PERLIN[k + 1 + g_TABELA_HASH_KEN_PERLIN[l + 1]]]] % 32;

	float n0, n1, n2, n3, n4;
	n0 = Contribution4D(gi0, x0, y0, z0, w0);
	n1 = Contribution4D(gi1, x1, y1, z1, w1);
	n2 = Contribution4D(gi2, x2, y2, z2, w2);
	n3 = Contribution4D(gi3, x3, y3, z3, w3);
	n4 = Contribution4D(gi4, x4, y4, z4, w4);

	return 27.0f * (n0 + n1 + n2 + n3 + n4);
}

float SimplexNoiseND(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	float* input = new float[nDim];

	const float SKEWING = (sqrt(nDim + 1) - 1) / nDim;
	const float UNSKEWING = (1 - 1 / (sqrt(nDim + 1))) / nDim;

	float s = 0;
	for (int i = 0; i < nDim; i++)
	{
		s += input[i];
	}
	s *= SKEWING;
	int* cellSkewed = new int[nDim];
	for (int i = 0; i < nDim; i++)
	{
		cellSkewed[i] = ParteaÎntreagăRapidf(input[i] + s);
	}

	float t = 0;
	for (int i = 0; i < nDim; i++)
	{
		t += input[i];
	}
	t *= UNSKEWING;
	int* cellUnskewed = new int[nDim];
	for (int i = 0; i < nDim; i++)
	{
		cellUnskewed[i] = (int)(cellSkewed[i] - t);
	}

	for (int i = 0; i < nDim; i++)
	{
		cellUnskewed[i] = (int)(input[i] - cellUnskewed[i]);
	}

	int* magnitudes = new int[nDim];
	for (int i = 0; i < nDim; i++)
	{
		for (int j = i + 1; j < nDim; j++)
		{
			if (cellUnskewed[i] > cellUnskewed[j])
			{
				magnitudes[i]++;
			}
			else
			{
				magnitudes[j]++;
			}
		}
	}

	int** simplexCorners = new int* [nDim];
	for (int i = 0; i < nDim - 1; i++)
	{
		simplexCorners[i] = new int[nDim];
		for (int j = 0; j < nDim; j++)
		{
			simplexCorners[i][j] = magnitudes[j] >= (3 - i) ? 1 : 0;
		}
	}
	for (int j = 0; j < nDim; j++)
	{
		simplexCorners[nDim - 1][j] = 1;
	}

	float** finalCorner = new float* [nDim];
	for (int i = 0; i < nDim; i++)
	{
		finalCorner[i] = new float[nDim];
		for (int j = 0; j < nDim; j++)
		{
			finalCorner[i][j] = cellUnskewed[j] - simplexCorners[i][j] + (i + 1) * UNSKEWING;
		}
	}

	for (int i = 0; i < nDim; i++)
	{
		cellSkewed[i] &= 255;
	}

	int modulusGradient = nDim * 2 << (nDim - 1); // numarul de muchii

	int* gradients = new int[nDim + 1];
	gradients[0] = 0;
	for (int i = nDim - 1; i >= 0; i--)
	{
		gradients[0] += g_TABELA_HASH_KEN_PERLIN[cellSkewed[i] + gradients[0]];
	}
	gradients[0] %= 32;
	for (int i = 0; i < nDim; i++)
	{
		gradients[i + 1] = 0;
		for (int j = 0; j < nDim; j++)
		{
			gradients[i + 1] += g_TABELA_HASH_KEN_PERLIN[(int)(cellSkewed[i] + finalCorner[i] + gradients[i + 1])];
		}
		gradients[i + 1] = 0;
	}

	float* contributions = new float[nDim + 1];
	float* noise = new float[nDim + 1];

	contributions[0] = 0.6;
	for (int j = 0; j < nDim; j++)
	{
		contributions[0] -= cellUnskewed[j] * cellUnskewed[j];
	}
	if (contributions[0] < 0)
	{
		noise[0] = 0;
	}
	else
	{
		contributions[0] *= contributions[0];
		noise[0] = contributions[0] * contributions[0] * ProdusScalarN(nDim, gradients, cellUnskewed);
	}
	for (int i = 1; i < nDim + 1; i++)
	{
		contributions[i] = 0.6;
		for (int j = 0; j < nDim; j++)
		{
			contributions[i] -= cellUnskewed[j] * cellUnskewed[j];
		}
		if (contributions[i] < 0)
		{
			noise[i] = 0;
		}
		else
		{
			contributions[i] *= contributions[i];
			noise[i] = contributions[i] * contributions[i] * ProdusScalarN(nDim, gradients, finalCorner[i]);
		}
	}

	const float someConstant = 70;
	int sum = 0;
	for (int i = 0; i < nDim + 1; i++)
	{
		sum += noise[i];
	}

	return someConstant * sum;
}