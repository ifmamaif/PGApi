using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class Utils
{
    public static bool OutSide(this Vector2 thisOne, Vector2 other)
    {
        return OutSide(thisOne.x, thisOne.y, other.x, other.y);
    }

    public static bool OutSide(this Vector2 thisOne, Vector2Int other)
    {
        return OutSide(thisOne.x, thisOne.y, other.x, other.y);
    }

    public static bool OutSide(this Vector2Int thisOne, Vector2Int other)
    {
        return OutSide(thisOne.x, thisOne.y, other.x, other.y);
    }

    public static bool OutSide(int x1,int y1,int x2,int y2)
    {
        return x1 >= x2
            || x1 < -x2
            || y1 >= y2
            || y1 < -y2;
    }

    public static bool OutSide(float x1, float y1, float x2, float y2)
    {
        return x1 >= x2
            || x1 < -x2
            || y1 >= y2
            || y1 < -y2;
    }
}
