#include "pch.h"
#include "KenPerlin.h"

#include "Math.h"
#include "Constants.h"
#include "Allocation.h"
#include "PerlinUtils.h"

float ZgomotulPerlin_Clasic1D(float x)
{
	int xIntreg = ParteaÎntreagăRapidf(x);
	
	x = x - xIntreg;

	xIntreg = xIntreg & 255;

	int gradient1 = g_TABELA_HASH_KEN_PERLIN[xIntreg] % 12;
	int gradient2 = g_TABELA_HASH_KEN_PERLIN[xIntreg + 1] % 12;
		
	float zgomot1 = ProdusScalarf((float)g_VECTORI_DIRECTIE[gradient1][0], x);
	float zgomot2 = ProdusScalarf((float)g_VECTORI_DIRECTIE[gradient2][0], x - 1);

	float valoreaEstompata = AmestecareHermitef(x);

	float valoareaFinalaZgomot = InterpolareLiniarăf(zgomot1, zgomot2, valoreaEstompata);

	return valoareaFinalaZgomot;
}

float ZgomotulPerlin_Clasic2D(float x, float y)
{
	/// I. Definirea unei rețele de vectori de gradient aleatori
	// Se rotunjesc parametrii de intrare la partea întreagă pentru a indexa în tabelă
	int xÎntreg = ParteaÎntreagăRapidf(x);
	int yÎntreg = ParteaÎntreagăRapidf(y);

	// Se ia partea fracționară pentru calcularea produsului scalar
	x = x - xÎntreg;
	y = y - yÎntreg;

	// Se restrâng valorile la 0 ... 255 deoarece tabela de hash are 256 de elemente
	xÎntreg = xÎntreg & 255;
	yÎntreg = yÎntreg & 255;

	// Se calculează punctele de gradient, sunt 4 puncte (8 în 3D)
	// Se face modulo 12 pentru indexarea in mulțimea "g_VECTORI_DIRECTIE" (de 12 elemente) de vectori de direcții
	int gradient1 = g_TABELA_HASH_KEN_PERLIN[xÎntreg + g_TABELA_HASH_KEN_PERLIN[yÎntreg]] % 12;
	int gradient2 = g_TABELA_HASH_KEN_PERLIN[xÎntreg + g_TABELA_HASH_KEN_PERLIN[yÎntreg + 1]] % 12;
	int gradient3 = g_TABELA_HASH_KEN_PERLIN[xÎntreg + 1 + g_TABELA_HASH_KEN_PERLIN[yÎntreg]] % 12;
	int gradient4 = g_TABELA_HASH_KEN_PERLIN[xÎntreg + 1 + g_TABELA_HASH_KEN_PERLIN[yÎntreg + 1]] % 12;
	
	// Se calculează câte o valoare de minim pentru parametrii de intrare (scăzând 1)
	float xMinim = x - 1;
	float yMinim = y - 1;

	/// II. Calcularea produsului scalar între vectorii de gradient și compensările acestora
	// Se calculează contribuțiile de zgomot din fiecare dintre cele 4 colțuri (8 în 3D) 
	// prin produsul scalar între vectorii de direcție și partea fracționara a parametrilor de intrare
	float contributieZgomot1 = ProdusScalarN(2, g_VECTORI_DIRECTIE[gradient1], x,		y);
	float contributieZgomot2 = ProdusScalarN(2, g_VECTORI_DIRECTIE[gradient2], x,		yMinim);
	float contributieZgomot3 = ProdusScalarN(2, g_VECTORI_DIRECTIE[gradient3], xMinim, y);
	float contributieZgomot4 = ProdusScalarN(2, g_VECTORI_DIRECTIE[gradient4], xMinim, yMinim);

	// III. Interpolare între valori
	// Se evaluează valoarea curbei de estompare folosind funcția Hermite pentru fiecare parametru de intrare
	float valoareEstompata1 = AmestecareHermitef(x);
	float valoareEstompata2 = AmestecareHermitef(y);

	// Interpolarea pe contribuțiile de zgomot
	// Prima serie de interpolări
	float zgomot1 = InterpolareLiniarăf(contributieZgomot1, contributieZgomot3, valoareEstompata1);
	float zgomot2 = InterpolareLiniarăf(contributieZgomot2, contributieZgomot4, valoareEstompata1);

	// A două și ultima interpolarea
	float zgomotFinal = InterpolareLiniarăf(zgomot1, zgomot2, valoareEstompata2);

	return zgomotFinal;
}

float FadeOriginal(float x)
{
	return (3 - 2 * x) * x * x;
}

float ZgomotulPerlin_Clasic3D(float x, float y, float z)
{
	int xi = ParteaÎntreagăRapidf(x);
	int yi = ParteaÎntreagăRapidf(y);
	int zi = ParteaÎntreagăRapidf(z);

	x = x - xi;
	y = y - yi;
	z = z - zi;

	xi = xi & 255;
	yi = yi & 255;
	zi = zi & 255;

	int gi000 = g_TABELA_HASH_KEN_PERLIN[xi + g_TABELA_HASH_KEN_PERLIN[yi + g_TABELA_HASH_KEN_PERLIN[zi]]] % 12;
	int gi001 = g_TABELA_HASH_KEN_PERLIN[xi + g_TABELA_HASH_KEN_PERLIN[yi + g_TABELA_HASH_KEN_PERLIN[zi + 1]]] % 12;
	int gi010 = g_TABELA_HASH_KEN_PERLIN[xi + g_TABELA_HASH_KEN_PERLIN[yi + 1 + g_TABELA_HASH_KEN_PERLIN[zi]]] % 12;
	int gi011 = g_TABELA_HASH_KEN_PERLIN[xi + g_TABELA_HASH_KEN_PERLIN[yi + 1 + g_TABELA_HASH_KEN_PERLIN[zi + 1]]] % 12;
	int gi100 = g_TABELA_HASH_KEN_PERLIN[xi + 1 + g_TABELA_HASH_KEN_PERLIN[yi + g_TABELA_HASH_KEN_PERLIN[zi]]] % 12;
	int gi101 = g_TABELA_HASH_KEN_PERLIN[xi + 1 + g_TABELA_HASH_KEN_PERLIN[yi + g_TABELA_HASH_KEN_PERLIN[zi + 1]]] % 12;
	int gi110 = g_TABELA_HASH_KEN_PERLIN[xi + 1 + g_TABELA_HASH_KEN_PERLIN[yi + 1 + g_TABELA_HASH_KEN_PERLIN[zi]]] % 12;
	int gi111 = g_TABELA_HASH_KEN_PERLIN[xi + 1 + g_TABELA_HASH_KEN_PERLIN[yi + 1 + g_TABELA_HASH_KEN_PERLIN[zi + 1]]] % 12;

	float xMin = x - 1;
	float yMin = y - 1;
	float zMin = z - 1;

	float n000 = ProdusScalarN(3, g_VECTORI_DIRECTIE[gi000], x, y, z);
	float n001 = ProdusScalarN(3, g_VECTORI_DIRECTIE[gi001], x, y, zMin);
	float n010 = ProdusScalarN(3, g_VECTORI_DIRECTIE[gi010], x, yMin, z);
	float n011 = ProdusScalarN(3, g_VECTORI_DIRECTIE[gi011], x, yMin, zMin);
	float n100 = ProdusScalarN(3, g_VECTORI_DIRECTIE[gi100], xMin, y, z);
	float n101 = ProdusScalarN(3, g_VECTORI_DIRECTIE[gi101], xMin, y, zMin);
	float n110 = ProdusScalarN(3, g_VECTORI_DIRECTIE[gi110], xMin, yMin, z);
	float n111 = ProdusScalarN(3, g_VECTORI_DIRECTIE[gi111], xMin, yMin, zMin);

	float u = AmestecareHermitef(x);
	float v = AmestecareHermitef(y);
	float w = AmestecareHermitef(z);

	float nx00 = InterpolareLiniarăf(n000, n100, u);
	float nx01 = InterpolareLiniarăf(n001, n101, u);
	float nx10 = InterpolareLiniarăf(n010, n110, u);
	float nx11 = InterpolareLiniarăf(n011, n111, u);

	float nxy0 = InterpolareLiniarăf(nx00, nx10, v);
	float nxy1 = InterpolareLiniarăf(nx01, nx11, v);

	float nxyz = InterpolareLiniarăf(nxy0, nxy1, w);

	return nxyz;
}

float ZgomotulPerlin_NDim(int nDim, ...)
{
	if (nDim < 1)
	{
		return 0;
	}

	float* input = new float[nDim];                                       
	
	VA_LIST_GET(nDim, float, input);

	float result = ZgomotulPerlin_NDimVector(nDim, input);

	delete[] input;

	return result;
}

float ZgomotulPerlin_NDimVector(int nDim, float* input)
{
	if (nDim < 1)
	{
		return 0;
	}

	const int N_DIM_MINUS_ONE = nDim - 1;
	const int N_DIM_PLUS_ONE = nDim + 1;
	const int NUMBER_OF_GRADIENT_INDICES = 1 << nDim;						// the number of gradient indices is = (2^number_of_dimensions)
	const int NUMBER_OF_EDGES = (nDim > 3)
		? (nDim * (1 << N_DIM_MINUS_ONE))									// the number of edges is = (number_of_dimensions * (2^(number_of_dimensions - 1)))
		: 12;																// special case when nDim is 3 or lower.

	int* unitGridCells = new int[nDim];                                  
	int* gradientIndices = new int[NUMBER_OF_GRADIENT_INDICES];          
	float* noiseContributions = new float[NUMBER_OF_GRADIENT_INDICES];   
	
	for (int i = 0; i < nDim; i++)
	{
		unitGridCells[i] = ParteaÎntreagăRapidf(input[i]);
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
		// each index will have the value = (permutation[x + 1_or_0(dependening_of_Index_important_bit) + permutation[y + 1_or_0 + permutation[z + 1_or_0 + ...]]] % number_Of_Indices)
		// x,y,z are the input from a point of 3rd Dimension.
		gradientIndices[i] = 0;
		for (int dim = 0; dim < nDim; dim++)
		{
			gradientIndices[i] = g_TABELA_HASH_KEN_PERLIN[(unitGridCells[(N_DIM_MINUS_ONE - dim)]) +
				VerificareBit(i, dim) +
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
				int scalar = VerificareBit(i, N_DIM_MINUS_ONE - j) ? -1 : 0;
				noiseContributions[i] += (input[j] + scalar) * g_VECTORI_DIRECTIE[gradientIndices[i]][j];
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
					: VerificareBit(i, nDim - 2 - (k++)) == 0 ? 1 : -1;
				int scalar = VerificareBit(i, N_DIM_MINUS_ONE - j) ? -1 : 0;
				noiseContributions[i] += (input[j] + scalar) * dot;
			}
		}
	}

	float* curves = input;
	for (int i = 0; i < nDim; i++)
	{
		curves[i] = CurbaEstomparef(input[i]);
	}

	int interpolations = NUMBER_OF_GRADIENT_INDICES / 2;
	for (int i = 0; i < nDim; i++)
	{
		for (int j = 0; j < interpolations; j++)
		{
			noiseContributions[j] = InterpolareLiniarăf(noiseContributions[j], noiseContributions[j + interpolations], curves[i]);
		}
		interpolations /= 2;
	}
	float result = noiseContributions[0];

	delete[] unitGridCells;
	delete[] gradientIndices;
	delete[] noiseContributions;

	return result;
}
