using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;


class NoisesDemo : GenericBehaviour
{
    public int width = 256;
    public int height = 256;
    public int scale = 20;
    public float offsetx = 100f;
    public float offsety = 100f;

    private const int numObj = 6;
    private GameObject[] objects = new GameObject[numObj];

    public override void Constructor()
    {
        for (int i = 0; i < numObj; i++)
        {
            var obj = GameObject.CreatePrimitive(PrimitiveType.Quad);
            obj.transform.parent = gameObject.transform;
            obj.transform.localScale = new Vector3(40, 40, 1);
            objects[i] = obj;

        }
        
        objects[0].transform.localPosition = new Vector3(-41f, 20, 0);
        objects[1].transform.localPosition = new Vector3(-41f, -20, 0);
        objects[2].transform.localPosition = new Vector3(0, 20, 0);
        objects[3].transform.localPosition = new Vector3(0, -20, 0);
        objects[4].transform.localPosition = new Vector3(41f, 20, 0);
        objects[5].transform.localPosition = new Vector3(41f, -20, 0);
    }

    public override void Generate()
    {
        GeneratePerlinNoiseClasic2D();
        GeneratePerlinNoiseClasic3D();
        GeneratePerlinNoiseImproved2D();
        GeneratePerlinNoiseImproved3D();
        GenerateSimplexNoise2D();
        GenerateSimplexNoise3D();
    }
    void GeneratePerlinNoiseClasic2D()
    {
        Renderer renderer = objects[0].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;

                float noise = PGApi.PGApi.ZgomotulPerlin_Clasic2D(xd, yd);
                noise = (noise + 1) / 2;
                Color color = new Color(noise, noise, noise);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GeneratePerlinNoiseClasic3D()
    {
        Renderer renderer = objects[1].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;

                float noise = PGApi.PGApi.ZgomotulPerlin_Clasic3D(xd, yd,0);
                noise = (noise + 1) / 2;
                Color color = new Color(noise, noise, noise);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GeneratePerlinNoiseImproved2D()
    {
        Renderer renderer = objects[2].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;

                float noise = PGApi.PGApi.ZgomotulPerlin_Îmbunătățit2D(xd, yd);
                noise = (noise + 1) / 2;
                Color color = new Color(noise, noise, noise);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GeneratePerlinNoiseImproved3D()
    {
        Renderer renderer = objects[3].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;

                float noise = PGApi.PGApi.ZgomotulPerlin_Îmbunătățit3D(xd, yd,0);
                noise = (noise + 1) / 2;

                Color color = new Color(noise, noise, noise);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GenerateSimplexNoise2D()
    {
        Renderer renderer = objects[4].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;

                float noise = PGApi.PGApi.ZgomotulPerlin_Simplex2D(xd, yd);
                noise = (noise + 1) / 2;

                Color color = new Color(noise, noise, noise);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GenerateSimplexNoise3D()
    {
        Renderer renderer = objects[5].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;

                float noise = PGApi.PGApi.ZgomotulPerlin_Simplex3D(xd, yd,0);
                noise = (noise + 1) / 2;

                Color color = new Color(noise, noise, noise);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

}