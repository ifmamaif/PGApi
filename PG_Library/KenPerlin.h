#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float ZgomotulPerlin_Clasic1D(float x);
	EXPORT_API float ZgomotulPerlin_Clasic2D(float x, float y);
	EXPORT_API float ZgomotulPerlin_Clasic3D(float x, float y, float z);

	EXPORT_API float ZgomotulPerlin_NDim(int nDim, ...);
	EXPORT_API float ZgomotulPerlin_NDimVector(int nDim, float* input);
}