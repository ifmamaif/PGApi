using Assets.Scripts;
using UnityEngine;  //	GameObject , Color

public class MDS : GenericBehaviour
{
	private GameObject boardHolder;
	private GameObject terrain;

	public int perlinWidth;
	public int perlinHeight;

	public float perlinScale = 1f;

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
	public bool usePGApi = false;

	private float textureSize = 1.24f;  // texturi 124 x 124
	private Vector3 initialPosition;
	private Renderer thisrenderer;


	// Use this for initialization
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

		boardHolder = new GameObject("Terrain");					//Instantiate Board and set boardHolder to its transform.
		boardHolder.transform.SetParent(this.transform);

		initialPosition = boardHolder.transform.position;

		terrain = GameObject.CreatePrimitive(PrimitiveType.Quad);   //	the land based on procedural generate by perlinnoise
		terrain.GetComponent<MeshCollider>().enabled = false;
		Material landMaterial = new Material(Shader.Find("Unlit/Texture"));
		landMaterial.name = "landMaterial";
		terrain.GetComponent<MeshRenderer>().material = landMaterial;
		terrain.name = "Land";
		terrain.transform.SetParent(boardHolder.transform);

		thisrenderer = terrain.GetComponent<Renderer>();

		SetRegions();
		//UpdateMap ();
	}

	public void Update()
	{
		if (autoMove || autoUpdate)
		{
			UpdateMap();
		}
	}

	public override void Generate()
	{
		perlinOffsetX = UnityEngine.Random.value * 1000;
		perlinOffsetY = UnityEngine.Random.value * 1000;
		UpdateMap();
	}

	public void Move(Vector2Int move, int speed)
	{
		if (move.x != 0)
		{
			boardHolder.transform.Translate(-move.x * textureSize / speed, 0, 0);

			if ((int)(boardHolder.transform.position.x / textureSize) != (int)(initialPosition.x / textureSize))
			{
				terrain.transform.Translate(move.x * textureSize, 0, 0);
				perlinOffsetX += perlinScale / perlinWidth * move.x;
				UpdateMap();
				initialPosition = boardHolder.transform.position;
			}

		}
		else if (move.y != 0)
		{
			boardHolder.transform.Translate(0, -move.y * textureSize / speed, 0);
			if ((int)(boardHolder.transform.position.y / textureSize) != (int)(initialPosition.y / textureSize))
			{
				terrain.transform.Translate(0, move.y * textureSize, 0);
				perlinOffsetY += perlinScale / perlinHeight * move.y;
				UpdateMap();
				initialPosition = boardHolder.transform.position;
			}
		}
	}

	void SetRegions()
	{
		regions = new TerrainType[8];
		regions[0].name = "Water Deep";
		regions[0].height = 0.3f;
		regions[0].colour = Utils.HexToColor("#3263C3");
		regions[1].name = "Water Shallow";
		regions[1].height = 0.4f;
		regions[1].colour = Utils.HexToColor("#3667C7");
		regions[2].name = "Sand";
		regions[2].height = 0.45f;
		regions[2].colour = Utils.HexToColor("#D2D07D");
		regions[3].name = "Grass";
		regions[3].height = 0.55f;
		regions[3].colour = Utils.HexToColor("#569817");
		regions[4].name = "Grass 2";
		regions[4].height = 0.6f;
		regions[4].colour = Utils.HexToColor("#3E6B12");
		regions[5].name = "Rock";
		regions[5].height = 0.7f;
		regions[5].colour = Utils.HexToColor("#5A453C");
		regions[6].name = "Rock 2";
		regions[6].height = 0.9f;
		regions[6].colour = Utils.HexToColor("#4B3C35");
		regions[7].name = "Snow";
		regions[7].height = 1f;
		regions[7].colour = Utils.HexToColor("#FFFFFF");
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
		float[,] noiseMap = usePGApi ?
			PGApi.Perlin.Generate2DMapUnity(perlinWidth, perlinHeight, perlinScale, perlinOffsetX, perlinOffsetY) :
			GenerateNoiseMap();

		Color[] colorMap = new Color[perlinWidth * perlinHeight];
		for (int y = 0; y < perlinHeight; y++)
		{
			for (int x = 0; x < perlinWidth; x++)
			{
				for (int i = 0; i < regions.Length; i++)
				{
					if (noiseMap[x, y] <= regions[i].height)
					{
						colorMap[y * perlinWidth + x] = regions[i].colour;
						break;
					}
				}
			}
		}
		return colorMap;
	}

	public float[,] GenerateNoiseMap()
	{
		float[,] noiseMap = new float[perlinWidth, perlinHeight];
		for (int y = 0; y < perlinHeight; y++)
		{
			for (int x = 0; x < perlinWidth; x++)
			{
				float xCoord = (float)x / perlinWidth * perlinScale + perlinOffsetX;
				float yCoord = (float)y / perlinHeight * perlinScale + perlinOffsetY;
				noiseMap[x, y] = Mathf.PerlinNoise(xCoord, yCoord);
			}
		}
		return noiseMap;
	}


}