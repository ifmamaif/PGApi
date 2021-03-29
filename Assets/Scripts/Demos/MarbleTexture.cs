using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;


class MarbleTexture : GenericBehaviour
{
    public int width = 256;
    public int height = 256;
    public int scale = 20;
    public float offsetx = 100f;
    public float offsety = 100f;

    public float pink_constant = 1;
    public float pink_frequency = 1;

    private const int numObj = 2;
    private GameObject[] objects = new GameObject[numObj];

    public override void Constructor()
    {
        for (int i = 0; i < numObj; i++)
        {
            var obj = GameObject.CreatePrimitive(PrimitiveType.Quad);
            obj.transform.parent = gameObject.transform;
            obj.transform.localScale = new Vector3(40, 40, 1);//Vector3.one;//new Vector3(0.88f,0.88f,1);
            objects[i] = obj;

        }

        objects[0].name = ("Sin");
        objects[1].name = ("Cos");

        objects[0].transform.localPosition = new Vector3(-41f, 20, 0);
        objects[1].transform.localPosition = new Vector3(41f, 20, 0);
    }

    public override void Generate()
    {
        GenerateTextureSin();
        GenerateTextureCos();
    }


    void GenerateTextureSin()
    {
        Renderer renderer = objects[0].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;

                float noise = PGApi.PGApi.PerlinNoise_Improved3D(xd, yd, 0);
                noise = Mathf.Sin(xd + noise);

                Color color = new Color(noise, noise, noise);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GenerateTextureCos()
    {
        Renderer renderer = objects[1].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;

                float noise = PGApi.PGApi.PerlinNoise_Improved3D(xd, yd, 0);
                noise = Mathf.Cos(xd + noise);

                Color color = new Color(noise, noise, noise);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

}