using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Assets.Scripts;
using System;

namespace Assets.Scripts
{
    public static class Utils
    {
        public static Color HexToColor(string hex)
        {
            ColorUtility.TryParseHtmlString(hex, out Color myColor);
            return myColor;
        }

        public static float Turbulence(int octaves, Vector3 P, float lacunarity, float gain, Func<float, float, float> noiseFunc)
        {
            float sum = 0;
            float scale = 1;
            float totalgain = 1;
            for (int i = 0; i < octaves; i++)
            {
                sum += totalgain * noiseFunc(P.x * scale, P.y * scale);//, P.z * scale);
                scale *= lacunarity;
                totalgain *= gain;
            }
            return Mathf.Abs(sum);
        }
    }
}
