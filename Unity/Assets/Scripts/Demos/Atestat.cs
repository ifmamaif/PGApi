using System;
using UnityEngine;  //  GameObject , Color

public class Atestat : GenericBehaviour
{
	private GameObject boardHolder = null;       //Instantiate Board and set boardHolder to its transform.
	private int Width = 20;
	private int Height = 20;
	private GameObject[,] terrainTexture = null;
	private int[,] terrainValue;
	private float textureSize = 0.64f;

	public override void Generate()
	{
		terrainValue = new int[Height, Width];
		Vector2Int playerPos = new Vector2Int(0, 0);

		DestroyOthers();

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

	private void DestroyOthers()
	{
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

		if (boardHolder != null)
		{
			Destroy(boardHolder);
		}

		terrainTexture = new GameObject[Height, Width];
		boardHolder = new GameObject("Terrain");
		boardHolder.transform.SetParent(gameObject.transform);
		boardHolder.transform.localScale = Vector3.one;
		boardHolder.transform.localPosition = Vector3.zero;
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
		//cell.transform.SetParent(boardHolder.transform);
		//var da = cell.GetComponent<Renderer>().bounds.size;
		//cell.transform.SetParent(boardHolder.transform);
		//float da = (float)Math.Round(cell.transform.localScale.x,2);
		//float newTextureSize = ((float)1 / da) * textureSize;
		//newTextureSize = (float)Math.Floor(newTextureSize);
		//textureSize = newTextureSize;
		//cell.transform.localScale = Vector3.one;
		//da = cell.GetComponent<Renderer>().bounds.size;
		//cell.transform.position = new Vector3(textureSize * y - Width / 2, -textureSize * x + Height / 2, 0.1f);
		//cell.transform.SetParent(boardHolder.transform);

		Texture2D cellTexture = (UnityEngine.Texture2D)Resources.Load("Resurse/" + value, typeof(Texture2D));
		if (cellTexture == null)
		{
			Debug.Log(x + " " + y + " " + value + " NULL");
			return null;
		}
		cellRenderer.sprite = Sprite.Create(cellTexture, new Rect(0.0f, 0.0f, cellTexture.width, cellTexture.height), new Vector2(0.5f, 0.5f));

		cell.transform.SetParent(boardHolder.transform);
		cell.transform.localScale = Vector3.one;
		cell.transform.localPosition = new Vector3(textureSize * (y - Width / 2), -textureSize * (x - Height / 2), 0f);

		return cell;
	}
}