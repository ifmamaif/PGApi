#include "pch.h"
#include "Math.h"

static const float EPSILON = 1e-7F;

#define Macro_Abs(x) x > 0 ? x : -x

int Absi(int value)
{
    return Macro_Abs(value);
}

float Absf(float value)
{
    return Macro_Abs(value);;
}

double Absd(double value)
{
    return Macro_Abs(value);
}

//float Floor(float f)
//{
//    int integer_f = (int)f;
//    if (Abs(f - integer_f) > EPSILON)
//    {
//        return f < 0.f ? integer_f + 1 : integer_f;
//    }
//    return f;
//}

int FloorToInt(float f)
{
    int integer_f = (int)f;
    if (Absf(f - integer_f) > EPSILON)
    {
        return f < 0 ? integer_f + 1 : integer_f;
    }
    return integer_f;
}

int RoundToInt(float f)
{
    const float HALF = 0.5f;
    if (f >= 0)
    {
        return (int)(f + HALF);
    }
    return (int)-(Absf(f) + HALF);
}

// Function to linearly interpolate between a0 and a1
// weight should be in the range [0.0, 1.0]
float Lerpf(float a0, float a1, float weight)
{
    // this is slightly faster equivalent formula for : (1.0 - w)*a0 + w * a1
    return a0 + weight * (a1 - a0);
}

// Function to linearly interpolate between a0 and a1
// weight should be in the range [0.0, 1.0]
double Lerpd(double a0, double a1, double weight)
{
    // this is slightly faster equivalent formula for : (1.0 - w)*a0 + w * a1
    return a0 + weight * (a1 - a0);
}