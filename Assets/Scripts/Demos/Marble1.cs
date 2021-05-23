using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Assets.Scripts;
using System;

public class Marble1 : PerlinNoisePreview
{
    public override void Constructor()
    {
        base.Constructor();

        scale = 5;
        offsetx = 0f;
        offsety = 0f;

        objects[0].name = ("Marble texture Classic");
        objects[1].name = ("Marble texture Improved");
    }

    protected override Color ColorFragmentShader(Vector3 v_texCoord3D, Func<float, float, float> noiseFunc)
    {
        Vector3 color1 = new Vector3(0.1f, 0.8f, 0.2f);
        Vector3 color2 = new Vector3(0.8f, 0.8f, 0.8f);

        var noise = Mathf.Cos(v_texCoord3D.z * 0.1f + 6f * Utils.Turbulence(5, v_texCoord3D, 2, 0.6f, noiseFunc));

        var result = new Color(
            Mathf.Lerp(color1.x, color2.x, noise),
            Mathf.Lerp(color1.y, color2.y, noise),
            Mathf.Lerp(color1.z, color2.z, noise),
            1);

        return result;
    }
}
