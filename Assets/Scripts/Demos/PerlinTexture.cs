﻿using UnityEngine;

class PerlinTexture : GenericBehaviour
{
    public int width = 256;
    public int height = 256;
    public int scale = 20;
    public float offsetx = 100f;
    public float offsety = 100f;

    public float pink_constant = 1;
    public float pink_frequency = 1;

    private const int numObj = 4;
    private GameObject[] objects = new GameObject[numObj];

    // Start is called before the first frame update
    public override void Constructor()
    {
        for(int i=0;i< numObj;i++)
        {
            var obj = GameObject.CreatePrimitive(PrimitiveType.Quad);
            obj.transform.parent = gameObject.transform;
            obj.transform.localScale = new Vector3(40, 40, 1);//Vector3.one;//new Vector3(0.88f,0.88f,1);
            objects[i] = obj;

        }

        objects[0].name = ("Perlin Noise");
        objects[1].name = ("Random");
        objects[2].name = ("RandomPos");
        objects[3].name = ("Sinus");

        objects[0].transform.localPosition = new Vector3(-41f, 20, 0);
        objects[1].transform.localPosition = new Vector3(-41f, -20, 0);
        objects[2].transform.localPosition = new Vector3(41f, -20, 0);
        objects[3].transform.localPosition = new Vector3(41f, 20, 0);

        Generate();
    }

    public override void Generate()
    {
        GenerateNormalNoise();
        //GenerateTextureRandom();
        GenerateTextureRandomPos();
        //GenerateTextureSin();

        da();

        GenerateTextureCosX();
        //GenerateTextureSinX();
    }

    public void GenerateNormalNoise()
    {
        Renderer renderer = objects[0].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        float[,] preColors = PGApi.Perlin.Generate2DMapUnity(width, height, scale, offsetx, offsety);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float value = (float)preColors[x, y];
                Color color = new Color(value, value, value);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GenerateTextureRandom()
    {
        Renderer renderer = objects[1].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;


                float noise = PGApi.PGApi.PerlinNoise_Improved3D(xd, yd, 0);
                               
                float rand = pink_constant / (float)noise;
                //rand = rand == float.PositiveInfinity ? float.MaxValue : rand == float.NegativeInfinity ? float.MinValue : rand;
                //rand = rand / float.MaxValue;
                //rand = (rand + 1) / 2;
                Color color = new Color(rand, rand, rand);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GenerateTextureRandomPos()
    {
        Renderer renderer = objects[1].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;


                float noise = PGApi.PGApi.PerlinNoise_Improved3D(xd, yd, 0);
                noise = noise < 0 ? noise * -1 : noise;

                float rand = (float)noise;// pink_constant / (float)noise;
                //rand = rand == float.PositiveInfinity ? float.MaxValue : rand == float.NegativeInfinity ? float.MinValue : rand;
                //rand = rand / float.MaxValue;
                //rand = (rand + 1) / 2;
                Color color = new Color(rand, rand, rand);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GenerateTextureSin()
    {
        Renderer renderer = objects[3].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;


                float noise = PGApi.PGApi.PerlinNoise_Improved3D(xd, yd, 0);
                noise = noise < 0 ? noise * -1 : noise;
                noise = Mathf.Lerp(0, Mathf.PI / 2, (float)noise);

                float rand = (float)noise;
                rand = Mathf.Sin(rand);
                Color color = new Color(rand, rand, rand);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GenerateTextureCosX()
    {
        Renderer renderer = objects[3].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;


                float noise = PGApi.PGApi.PerlinNoise_Improved3D(xd, yd, 0);
                float rand = (float)(xd + noise);
                rand = Mathf.Cos(rand);
                Color color = new Color(rand, rand, rand);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void GenerateTextureSinX()
    {
        Renderer renderer = objects[2].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width * scale + offsetx;
                float yd = (float)y / height * scale + offsety;


                float noise = PGApi.PGApi.PerlinNoise_Improved3D(xd, yd, 0);
                float rand = (float)(xd + noise);
                rand = Mathf.Sin(rand);
                Color color = new Color(rand, rand, rand);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    void da()
    {
        Renderer renderer = objects[2].GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float xd = (float)x / width ;
                float yd = (float)y / height;
                float noise = ((float)PGApi.PGApi.PerlinNoise_Improved3D(xd, yd, 0)+1/2);

                float rand = (xd + yd ) / 2;
                rand = (rand + noise) / 2;

                Color color = new Color(rand, rand, rand);
                texture.SetPixel(x, y, color);
            }

        texture.Apply();
        renderer.material.mainTexture = texture;
    }

    // Update is called once per frame
    public void Update()
    {
        Generate();
    }
}
