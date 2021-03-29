#pragma once


float Fadef(float t);

// Helper function to compute gradients-dot-residual vectors (1D)
//
// note that these generate gradients of more than unit length.
// To make a close match with the value range of classic Perlin noise,
// the final noise values need to be rescaled to fit nicely within[-1, 1].
// (The simplex noise functions as such also have different scaling.)
// Note also that these noise functions are the most practical and useful
// signed version of Perlin noise.
float Gradient1D(int hash, float x);
float Gradient2D(int hash, float x, float y);
// same function that Unity3D use
float Gradient3D(int hash, float x, float y, float z);