using Assets.Scripts;
using UnityEngine;  //	GameObject , Color

public class EarthMap : GenericBehaviour
{
    private GameObject boardHolder;
    private GameObject terrain;

    public int perlinWidth;
    public int perlinHeight;

    public float perlinScale = 20f;

    public float perlinOffsetX = 0f;
    public float perlinOffsetY = 0f;

    [System.Serializable]
    public struct TerrainType
    {
        public string name;
        public float height;
        public Color colour;
    }
    public TerrainType[] regions;

    public bool autoUpdate = false;
    public bool autoMove = false;

    private float textureSize = 1.24f;  // texturi 124 x 124
    private Renderer thisrenderer;

    public override void Constructor()
    {
        perlinWidth = 90;
        perlinHeight = 90;

        perlinOffsetX = 100;
        perlinOffsetY = 100;

        if (boardHolder)
        {
            return;
        }

        boardHolder = new GameObject("Terrain");                    //Instantiate Board and set boardHolder to its transform.
        boardHolder.transform.SetParent(this.transform);

        terrain = GameObject.CreatePrimitive(PrimitiveType.Quad);   //	the land based on procedural generate by perlinnoise
        terrain.GetComponent<MeshCollider>().enabled = false;
        Material landMaterial = new Material(Shader.Find("Unlit/Texture"));
        landMaterial.name = "landMaterial";
        terrain.GetComponent<MeshRenderer>().material = landMaterial;
        terrain.name = "Land";
        terrain.transform.SetParent(boardHolder.transform);

        thisrenderer = terrain.GetComponent<Renderer>();

        SetRegions();
    }

    void SetRegions()
    {
        regions = new TerrainType[3];
        regions[0].name = "Water";
        regions[0].height = 0.5f;
        regions[0].colour = new Color(0,0,2,1);
        regions[1].name = "Normal Terrain";
        regions[1].height = 0.9f;
        regions[1].colour = new Color(0,1,0.5f,1);
        regions[2].name = "Snow";
        regions[2].height = 1f;
        regions[2].colour = new Color(1, 1, 1, 1);
    }

    public void Update()
    {
        if (autoMove || autoUpdate)
        {
            UpdateMap();
        }
    }

    void UpdateMap()
    {
        if (autoMove == true)
        {
            perlinOffsetX += perlinScale / perlinWidth;
        }

        if (terrain.transform.localScale != new Vector3(perlinWidth * textureSize, perlinHeight * textureSize, 1))
        {
            terrain.transform.localScale = new Vector3(perlinWidth * textureSize, perlinHeight * textureSize, 1);
        }
        thisrenderer.material.mainTexture = TextureFromcolorMap();
    }

    public Texture2D TextureFromcolorMap()
    {
        Color[] colorMap = GeneratecolorMap();
        Texture2D texture = new Texture2D(perlinWidth, perlinHeight)
        {
            filterMode = FilterMode.Point,
            wrapMode = TextureWrapMode.Clamp
        };
        texture.SetPixels(colorMap);
        texture.Apply();
        return texture;
    }

    public Color[] GeneratecolorMap()
    {
        float[,] noiseMap = new float[perlinWidth, perlinHeight];

        for (int y = 0; y < perlinHeight; y++)
        {
            for (int x = 0; x < perlinWidth; x++)
            {
                //float xCoord = (float)x / perlinWidth * perlinScale + perlinOffsetX;
                //float yCoord = (float)y / perlinHeight * perlinScale + perlinOffsetY;
                float xCoord = (float)x * perlinScale + perlinOffsetX;
                float yCoord = (float)y  * perlinScale + perlinOffsetY;
                var noise = PGApi.PGApi.Fbm(xCoord, yCoord, 0, 8, 1, 0.5f, 0.0005f, 2);
                noise *= 2f;
                noise += 1;
                noise /= 2;
                if (noise > 1)
                    noise = 1;
                noiseMap[x, y] = noise;
            }
        }

        Color[] colorMap = new Color[perlinWidth * perlinHeight];
        for (int y = 0; y < perlinHeight; y++)
        {
            for (int x = 0; x < perlinWidth; x++)
            {
                for (int i = 0; i < regions.Length; i++)
                {
                    if (noiseMap[x, y] <= regions[i].height)
                    {
                        var noise = noiseMap[x, y];
                        var color = regions[i].colour;
                        color *= noise;
                                               
                        colorMap[y * perlinWidth + x] = color;
                        break;
                    }
                }
            }
        }
        return colorMap;
    }

    public override void Generate()
    {
        perlinOffsetX = UnityEngine.Random.value * 1000;
        perlinOffsetY = UnityEngine.Random.value * 1000;
        UpdateMap();
    }
}
