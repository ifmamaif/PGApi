#include "pch.h"
#include "Utils.h"

bool Utils::OutSide(int x1, int y1, int x2, int y2)
{
    return x1 >= x2
        || x1 < -x2
        || y1 >= y2
        || y1 < -y2;
}

bool Utils::OutSide(float x1, float y1, float x2, float y2)
{
    return x1 >= x2
        || x1 < -x2
        || y1 >= y2
        || y1 < -y2;
}