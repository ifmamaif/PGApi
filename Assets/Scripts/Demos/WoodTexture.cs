using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Assets.Scripts;
using System;

public class WoodTexture : PerlinNoisePreview
{
    public override void Constructor()
    {
        base.Constructor();

        scale = 5;
        offsetx = 0f;
        offsety = 0f;
    }

    protected override Color ColorFragmentShader(Vector3 v_texCoord3D, Func<float, float, float> noiseFunc)
    {
        Vector3 color1 = new Vector3(0.658f, 0.356f, 0.0588f);
        Vector3 color2 = new Vector3(0.952f, 0.713f, 0.4700f);

        float x_v = v_texCoord3D.x * 0.6f;
        float y_v = v_texCoord3D.y * 0.6f;
        float z_v = v_texCoord3D.z * 0.6f;

        Vector3 t1 = new Vector3(x_v * 2, y_v * 2, z_v * 2);
        Vector3 t2 = new Vector3(0, 0, 0);
        
        float temp_x = 60 * noiseFunc(t1.x, t1.y);
        float temp_y = 60 * noiseFunc(t2.x, t2.y);
        
        x_v -= temp_x;
        y_v -= temp_y;

        var noise = Mathf.Pow(Mathf.Cos(Mathf.Sqrt(x_v * x_v + y_v * y_v) + 30 * Utils.Turbulence(68, t2, 2, 0.6f, noiseFunc)) * 0.5f + 0.5f, 3);

        var result = new Color(
            Mathf.Lerp(color1.x, color2.x, noise),
            Mathf.Lerp(color1.y, color2.y, noise),
            Mathf.Lerp(color1.z, color2.z, noise),
            1);
        
        return result;
    }
}
