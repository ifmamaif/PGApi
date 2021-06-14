#include "pch.h"
#include "Math.h"

static const float EPSILON = 1e-7F;

#define Macro_Abs(x) x > 0 ? x : -x

int AbsolutÎntregi(int value)
{
    return Macro_Abs(value);
}

float AbsolutÎntregf(float value)
{
    return Macro_Abs(value);
}

double AbsolutÎntregd(double value)
{
    return Macro_Abs(value);
}

int ParteaÎntreagăf(float f)
{
    int integer_f = (int)f;
    if (AbsolutÎntregf(f - integer_f) > EPSILON)
    {
        return f < 0 ? integer_f + 1 : integer_f;
    }
    return integer_f;
}

int ParteaÎntreagăd(double f)
{
	int integer_d = (int)f;
	if (AbsolutÎntregd(f - integer_d) > EPSILON)
	{
		return f < 0 ? integer_d + 1 : integer_d;
	}
	return integer_d;
}

int RotunjireLaÎntregf(float f)
{
    const float HALF = 0.5f;
    if (f >= 0)
    {
        return (int)(f + HALF);
    }
    return (int)-(AbsolutÎntregf(f) + HALF);
}

float InterpolareLiniarăf(float a, float b, float greutate)
{
    return a + greutate * (b - a);
}

double InterpolareLiniarăd(double a0, double a1, double weight)
{
    return a0 + weight * (a1 - a0);
}

int ParteaÎntreagăRapidf(float d)
{
    return d > 0 ? (int)d : (int)d - 1;
}

int ParteaÎntreagăRapidd(double d)
{
	return d > 0 ? (int)d : (int)d - 1;
}

int ProdusScalari(int x, int y)
{
    return x * y;
}

float ProdusScalarf(float x, float y)
{
	return x * y;
}

float ProdusScalarN(int nDim, const int g[], ... )
{
    float result = 0;
	va_list list;
	va_start(list, g);
	for (int i = 0; i < nDim; i++)
	{
        float elem = (float)va_arg(list, double);

        result = result + (g[i] * (elem));
	}
	va_end(list);
    return result;
}

float ProdusScalarVector(int nDim, const int g[], const float x[])
{
    float result = 0;
	for (int i = 0; i < nDim; i++)
	{
		result = result + (g[i] * x[i]);
	}
	return result;
}

int VerificareBit(int number, int bitPosition)
{
    int bitChecker = 1 << bitPosition;
    int si = number & bitChecker;
    int result = si > 0 ? 1 : 0;
    return result;
}