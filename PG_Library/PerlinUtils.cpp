#include "pch.h"
#include "PerlinUtils.h"

float CurbaEstomparef(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float AmestecareHermitef(float t)
{
	return (t * t) * (3 - 2 * t);
}

float Gradient1D(int hash, float x)
{
	const int h = hash & 15;
	float grad = 1.0f + (h & 7);
	if ((h & 8) != 0) grad = -grad;
	return (grad * x);
}

float Gradient2D(int hash, float x, float y)
{
	///	Calcularea gradientului pe un colț al cubului

	const int CODUL_HASH = hash & 63;									// Constrângere la 6 biți pentru codul hash.
	const float COORDONATA1 = CODUL_HASH < 4 ? x : y;					// Se ia coordonată x sau y în funcție de codul hash
	const float COORDONATA2 = CODUL_HASH < 4 ? y : x;

	return ((CODUL_HASH & 1) ? -COORDONATA1 : COORDONATA1) +
		((CODUL_HASH & 2) ? -2.0f * COORDONATA2 : 2.0f * COORDONATA2);	// Calcularea produsului scalar cu x și y
}

float Gradient3D(int hash, float x, float y, float z)
{
	int h = hash & 15;
	float u = h < 8 ? x : y;
	float v;
	if (h < 4)
		v = y;
	else if (h == 12 || h == 14)
		v = x;
	else
		v = z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float Gradient1D_Imbunatit(int hash, float x)
{
	switch (hash & 0xF)
	{
	case 0x0: return      x;
	case 0x1: return  2 * x;
	case 0x2: return  3 * x;
	case 0x3: return  4 * x;
	case 0x4: return  5 * x;
	case 0x5: return  6 * x;
	case 0x6: return  7 * x;
	case 0x7: return  8 * x;
	case 0x8: return -1 * x;
	case 0x9: return -2 * x;
	case 0xA: return -3 * x;
	case 0xB: return -4 * x;
	case 0xC: return -5 * x;
	case 0xD: return -6 * x;
	case 0xE: return -7 * x;
	case 0xF: return -8 * x;
	default:
		return 0; // never happens
	}
}

float Gradient2D_Imbunatit(int codulHash, float x, float y)
{
	codulHash = codulHash & 0xF;

	switch (codulHash)
	{
	case 0x0: return  x + 2 * y;
	case 0x1: return -x + 2 * y;
	case 0x2: return  x - 2 * y;
	case 0x3: return -x - 2 * y;
	default:
		break;
	}

	codulHash = codulHash & 3;
	switch (codulHash)
	{
	case 0x0: return  2 * x + y;
	case 0x1: return  2 * x - y;
	case 0x2: return -2 * x + y;
	case 0x3: return -2 * x - y;
	default:
		return 0;
	}
}

float Gradient3D_Improved(int hash, float x, float y, float z)
{
	switch (hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0; // never happens
	}
}