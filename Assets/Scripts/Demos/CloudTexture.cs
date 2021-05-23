using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Assets.Scripts;
using System;
public class CloudTexture : PerlinNoisePreview
{
    public override void Constructor()
    {
        base.Constructor();
        scale = 3;
        offsetx = 0f;
        offsety = 0f;
    }

    protected override Color ColorFragmentShader(Vector3 v_texCoord3D, Func<float, float, float> noiseFunc)
    {
        Vector3 color1 = new Vector3(0.1f, 0.1f, 0.99f);
        Vector3 color2 = new Vector3(0.8f, 0.8f, 0.80f);

        var noise = Mathf.Cos(v_texCoord3D.z * 0.5f + 2 * Utils.Turbulence(6, v_texCoord3D, 2, 0.5f, noiseFunc)) * 0.9f;

        var result = new Color(
            Mathf.Lerp(color1.x, color2.x, noise),
            Mathf.Lerp(color1.y, color2.y, noise),
            Mathf.Lerp(color1.z, color2.z, noise),
            1);

        return result;
    }
}
