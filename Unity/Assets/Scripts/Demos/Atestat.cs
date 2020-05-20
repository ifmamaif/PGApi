using UnityEngine;  //  GameObject , Color

public class Atestat : MonoBehaviour
{
	private GameObject boardHolder;       //Instantiate Board and set boardHolder to its transform.
	private int Width = 10;
	private int Height = 10;
	private GameObject[,] terrainTexture;
	private int[,] terrainValue;
	private readonly float textureSize = 0.64f;

	public GameObject parent;

	// Start is called before the first frame update
	public void Start()
	{
		Height = 20;
		Width = 20;
		terrainValue = new int[Height, Width];
		terrainTexture = new GameObject[Height, Width];
		//boardHolder = new GameObject("Terrain");
		//boardHolder.transform.localScale = new Vector3(1, 1, 1);

		GenerareLabirint();

		//boardHolder = gameObject;

		//parent = gameObject;
		//if (parent)
		//    boardHolder.transform.SetParent(parent.transform);
	}

	public void GenerareLabirint()
	{
		Vector2Int playerPos = new Vector2Int(0, 0);

		if (terrainTexture != null)
		{
			for (int i = 0; i < Height; i++)
				for (int j = 0; j < Width; j++)
				{
					if (terrainTexture[i, j] != null)
					{
						UnityEngine.Object.Destroy(terrainTexture[i, j]);
					}
				}
		}

		int randomNumber;
		System.Random randomGenerator = new System.Random();
		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				randomNumber = randomGenerator.Next(1, 101); // de la 1 la 100
				if (randomNumber < 66)
				{
					terrainValue[i, j] = 1;
				}
				else
				{
					terrainValue[i, j] = 0;
				}
			}
		}
		terrainValue[playerPos.y, playerPos.x] = 1;

		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				terrainTexture[i, j] = NewCell(i, j, terrainValue[i, j].ToString());
			}
		}
	}

	private GameObject NewCell(int x, int y, string value)
	{
		if (value == null || value == "")
		{
			Debug.Log(x + " " + y + " " + value + " value: NULL");
			return null;
		}

		GameObject cell = new GameObject(x + " " + y);
		cell.transform.localScale = Vector3.one;
		SpriteRenderer cellRenderer = cell.AddComponent(typeof(SpriteRenderer)) as SpriteRenderer;
		cell.transform.SetParent(gameObject.transform);
		cell.transform.position = new Vector3(textureSize * y - Width / 2, -textureSize * x + Height / 2, 0.1f);

		Texture2D cellTexture = (UnityEngine.Texture2D)Resources.Load("Resurse/" + value, typeof(Texture2D));
		if (cellTexture == null)
		{
			Debug.Log(x + " " + y + " " + value + " NULL");
			return null;
		}
		cellRenderer.sprite = Sprite.Create(cellTexture, new Rect(0.0f, 0.0f, cellTexture.width, cellTexture.height), new Vector2(0.5f, 0.5f));

		return cell;
	}
}