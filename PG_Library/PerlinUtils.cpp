#include "pch.h"
#include "PerlinUtils.h"

float Fadef(float t)
{
	// Fade function as defined by Ken Perlin.  
	// This eases coordinate values so that they will "ease" towards integral values. 
	// This ends up smoothing the const output.
	// 6*t^5 - 15*t^4 + 10*t^3
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float HermiteBlendingFunctionf(float t)
{
	// Fade function as defined by Ken Perlin. 
	// 3*t^2 - 2*t^3
	// equivalent: t^2 * (3 - 2 * t)
	return (t * t) * (3 - 2 * t);
}

float Gradient1D(int hash, float x)
{
	const int h = hash & 15;		// Convert low 4 bits of hash code , 15 = 1111
	float grad = 1.0f + (h & 7);    // Gradient value 1.0, 2.0, ..., 8.0
	if ((h & 8) != 0) grad = -grad; // Set a random sign for the gradient
//  float grad = gradients1D[h];    // NOTE : Test of Gradient look-up table instead of the above
	return (grad * x);              // Multiply the gradient with the distance
}

float Gradient2D(int hash, float x, float y) 
{
	const int h = hash & 63;										// Convert low 3 bits of hash code
	const float u = h < 4 ? x : y;									// into 8 simple gradient directions,
	const float v = h < 4 ? y : x;
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);	// and compute the dot product with (x,y).
}

float Gradient3D(int hash, float x, float y, float z)
{
	// Convert low 4 bits of hash code into 12 simple
	// CONVERT LO 4 BITS OF HASH CODE
	int h = hash & 15  /* 0b1111 */;                            // Take the hashed value and take the first 4 bits of it (15 == 0b1111)

	// gradient directions, and compute dot product.
	// INTO 12 GRADIENT DIRECTIONS.
	float u = h < 8 /* 0b1000 */ ? x : y;						// If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.

	// Fix repeats at h = 12 to 15
	float v;													// In Ken Perlin's original implementation this was another conditional operator (?:).
	if (h < 4 /* 0b0100 */)                                     // If the first and second significant bits are 0 set v = y
		v = y;
	else if (h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)		// If the first and second significant bits are 1 set v = x
		v = x;
	else                                                        // If the first and second significant bits are not equal (0/1, 1/0) set v = z
		v = z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);	// Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
}


float Gradient1D_Improved(int hash, float x) 
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

float Gradient2D_Improved(int hash, float x, float y)
{
	hash = hash & 0xF;
	
	switch (hash)
	{
	case 0x0: return  x + 2 * y;
	case 0x1: return -x + 2 * y;
	case 0x2: return  x - 2 * y;
	case 0x3: return -x - 2 * y;
	default:
		break;
	}	

	hash = hash & 3;
	switch (hash)
	{
	case 0x0: return  2 * x + y;
	case 0x1: return  2 * x - y;
	case 0x2: return -2 * x + y;
	case 0x3: return -2 * x - y;
	default:
		return 0; // never happens
	}
}

// Source: http://riven8192.blogspot.com/2010/08/calculate-perlinnoise-twice-as-fast.html
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