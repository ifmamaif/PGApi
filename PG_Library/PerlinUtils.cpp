#include "pch.h"
#include "PerlinUtils.h"

float Fade(float t)
{
	// Fade function as defined by Ken Perlin.  
	// This eases coordinate values so that they will "ease" towards integral values. 
	// This ends up smoothing the const output.
	// 6*t^5 - 15*t^4 + 10*t^3
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float Gradient1D(int hash, float x) {
	const int h = hash & 15;		// Convert low 4 bits of hash code , 15 = 1111
	float grad = 1.0f + (h & 7);    // Gradient value 1.0, 2.0, ..., 8.0
	if ((h & 8) != 0) grad = -grad; // Set a random sign for the gradient
//  float grad = gradients1D[h];    // NOTE : Test of Gradient look-up table instead of the above
	return (grad * x);              // Multiply the gradient with the distance
}

float Gradient2D(int hash, float x, float y) {
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
	float v;													// In Ken Perlin's original implementation this was another conditional operator (?:).  I
																// expanded it for readability.
	if (h < 4 /* 0b0100 */)                                     // If the first and second significant bits are 0 set v = y
		v = y;
	else if (h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)		// If the first and second significant bits are 1 set v = x
		v = x;
	else                                                        // If the first and second significant bits are not equal (0/1, 1/0) set v = z
		v = z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);	// Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
}