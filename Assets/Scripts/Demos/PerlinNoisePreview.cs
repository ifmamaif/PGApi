using UnityEngine;

public class PerlinNoisePreview : GenericBehaviour
{
    public int width = 256;
    public int height = 256;
    public int scale = 20;
    public float offsetx = 100f;
    public float offsety = 100f;

    private GameObject perlinObject;
    private GameObject randomObject;
    
    // Start is called before the first frame update
    public override void Constructor()
    {
        perlinObject = GameObject.CreatePrimitive(PrimitiveType.Quad);
        randomObject = GameObject.CreatePrimitive(PrimitiveType.Quad);

        perlinObject.name = ("Perlin Noise");
        randomObject.name = ("Random");

        perlinObject.transform.parent = gameObject.transform;
        randomObject.transform.parent = gameObject.transform;

        perlinObject.transform.localPosition = new Vector3(0.51f, 0, 0.9f);
        randomObject.transform.localPosition = new Vector3(-0.51f, 0, 0.9f);

        perlinObject.transform.localScale = Vector3.one;
        randomObject.transform.localScale = Vector3.one;

        Generate();
        GenerateTextureRandom();
    }

    public override void Generate()
    {
        Renderer renderer = perlinObject.GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        double[,] preColors = PGApi.Perlin.Generate2DMapUnity(width, height, scale, offsetx, offsety);

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
        Renderer renderer = randomObject.GetComponent<MeshRenderer>();
        Texture2D texture = new Texture2D(width, height);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                float rand = UnityEngine.Random.value;
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