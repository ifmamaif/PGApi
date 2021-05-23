using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Assets.Scripts;
using System;
public class FurTexture : PerlinNoisePreview
{
    public override void Constructor()
    {
        base.Constructor();
        scale = 3;
        offsetx = 0f;
        offsety = 0f;
    }

    float clamp(float d, float min, float max)
    {
        var t = d < min ? min : d;
        return t > max ? max : t;
    }

    protected override Color ColorFragmentShader(Vector3 v_texCoord3D, Func<float, float, float> noiseFunc)
    {
        //Vector3 color1 = new Vector3(0.8f, 0.7f, 0.0f); // galben
        //Vector3 color2 = new Vector3(0.6f, 0.1f, 0.0f); // rosu
        Vector3 color1 = new Vector3(0.6f, 0.3f, 0.0f); // maro
        Vector3 color2 = new Vector3(0.2f, 0.1f, 0.0f); // maro inchis

        var noise = clamp(0.7f * Utils.Turbulence(6, v_texCoord3D, 3, 0.9f, noiseFunc), 0, 1);

        var result = new Color(
            Mathf.Lerp(color1.x, color2.x, noise),
            Mathf.Lerp(color1.y, color2.y, noise),
            Mathf.Lerp(color1.z, color2.z, noise),
            1);

        return result;

    }
}
