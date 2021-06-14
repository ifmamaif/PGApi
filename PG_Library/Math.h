#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API int AbsolutÎntregi(int value);
	EXPORT_API float AbsolutÎntregf(float value);
	EXPORT_API double AbsolutÎntregd(double value);

	EXPORT_API int ParteaÎntreagăRapidf(float d);
	EXPORT_API int ParteaÎntreagăRapidd(double d);

	EXPORT_API int ParteaÎntreagăf(float f);
	EXPORT_API int ParteaÎntreagăd(double f);

	EXPORT_API int RotunjireLaÎntregf(float f);

	// Interpolare liniară între a și b
	// greutatea trebuie să fie în intervalul închis [0.0, 1.0]
	EXPORT_API float InterpolareLiniarăf(float a, float b, float greutate);
	EXPORT_API double InterpolareLiniarăd(double a0, double a1, double weight);

	EXPORT_API int ProdusScalari(int x, int y);
	EXPORT_API float ProdusScalarf(float x, float y);
	EXPORT_API float ProdusScalarN(int nDim, const int g[], ...);
	EXPORT_API float ProdusScalarVector(int nDim, const int g[], const float x[]);

	EXPORT_API int VerificareBit(int number, int bitPosition);
}