#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API float AmestecareHermitef(float t);
	EXPORT_API float CurbaEstomparef(float t);

	EXPORT_API float Gradient1D(int hash, float x);
	EXPORT_API float Gradient2D(int hash, float x, float y);
	EXPORT_API float Gradient3D(int hash, float x, float y, float z);
}