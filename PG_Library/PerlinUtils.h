#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float HermiteBlendingFunctionf(float t);
	EXPORT_API float Fadef(float t);

	// Helper function to compute gradients-dot-residual vectors (1D)
	//
	// note that these generate gradients of more than unit length.
	// To make a close match with the value range of classic Perlin noise,
	// the final noise values need to be rescaled to fit nicely within[-1, 1].
	// (The simplex noise functions as such also have different scaling.)
	// Note also that these noise functions are the most practical and useful
	// signed version of Perlin noise.
	EXPORT_API float Gradient1D(int hash, float x);
	EXPORT_API float Gradient2D(int hash, float x, float y);
	// same function that Unity3D use
	EXPORT_API float Gradient3D(int hash, float x, float y, float z);
}