using System;
using UnityEngine;

public class PerlinNoisePreview : GenericBehaviour
{
    public int width = 256;
    public int height = 256;
    public int scale = 20;
    public float offsetx = 100f;
    public float offsety = 100f;

    protected const int numObj = 4;
    protected GameObject[] objects = new GameObject[numObj];

    // Start is called before the first frame update
    public override void Constructor()
    {
        for (int i = 0; i < numObj; i++)
        {
            var gameObj = GameObject.CreatePrimitive(PrimitiveType.Quad);
            gameObj.transform.parent = gameObject.transform;
            gameObj.transform.localScale = new Vector3(40, 40, 1);
            objects[i] = gameObj;
        }

        objects[0].name = ("Random");
        objects[0].transform.localPosition = new Vector3(-41f, 21, 0);

        objects[1].name = ("Perlin Noise");
        objects[1].transform.localPosition = new Vector3(41f, 21, 0);

        objects[2].name = ("Perlin Noise Improved");
        objects[2].transform.localPosition = new Vector3(-41f, -21, 0);

        objects[3].name = ("Simplex Noise");
        objects[3].transform.localPosition = new Vector3(41f, -21, 0);
    }
    
    public override void Generate()
    {
        GenerateTexture(0, Rand);
        GenerateTexture(1, PGApi.PGApi.ZgomotulPerlin_Clasic2D);
        GenerateTexture(2, PGApi.PGApi.ZgomotulPerlin_Îmbunătățit2D);
        GenerateTexture(3, PGApi.PGApi.ZgomotulPerlin_Simplex2D);
    }

    void GenerateTexture(int indexObj, Func<float, float, float> noiseFunc)
    {
        Renderer renderer = objects[indexObj].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;

                var color = ColorFragmentShader(new Vector3(xd, yd, 0), noiseFunc);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    float Rand(float x, float y)
    {
        return UnityEngine.Random.value;
    }

    float Rand(float x,float y,float z)
    {
        return UnityEngine.Random.value;
    }

    protected virtual Color ColorFragmentShader(Vector3 v_texCoord3D, Func<float, float, float> noiseFunc)
    {
        var noise = noiseFunc(v_texCoord3D.x, v_texCoord3D.y);
        return new Color(noise, noise, noise, 1f);
    }


}