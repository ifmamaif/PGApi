using System;
using UnityEngine;  //  GameObject , Color

public class Atestat : GenericBehaviour
{
	private GameObject m_BoardHolder = null;       //Instantiate Board and set boardHolder to its transform.
	private readonly int m_Width = 20;
	private readonly int m_Height = 20;
	private GameObject[,] m_TerrainTexture = null;
	private const float TEXTURE_SIZE = 0.64f;

	public override void Generate()
	{
		int[,] m_TerrainValue = DumbLabirint.GenerateDumbMaze(m_Width, m_Height);

		DestroyOthers();

		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				m_TerrainTexture[i, j] = NewCell(i, j, m_TerrainValue[i, j].ToString());
			}
		}
	}

	private void DestroyOthers()
	{
		if (m_TerrainTexture != null)
		{
			for (int i = 0; i < m_Height; i++)
				for (int j = 0; j < m_Width; j++)
				{
					if (m_TerrainTexture[i, j] != null)
					{
						UnityEngine.Object.Destroy(m_TerrainTexture[i, j]);
					}
				}
		}

		if (m_BoardHolder != null)
		{
			Destroy(m_BoardHolder);
		}

		m_TerrainTexture = new GameObject[m_Height, m_Width];
		m_BoardHolder = new GameObject("Terrain");
		m_BoardHolder.transform.SetParent(gameObject.transform);
		m_BoardHolder.transform.localScale = Vector3.one;
		m_BoardHolder.transform.localPosition = Vector3.zero;
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

		Texture2D cellTexture = (Texture2D)Resources.Load("Resurse/" + value, typeof(Texture2D));
		if (cellTexture == null)
		{
			Debug.Log(x + " " + y + " " + value + " NULL");
			return null;
		}
		cellRenderer.sprite = Sprite.Create(cellTexture, new Rect(0.0f, 0.0f, cellTexture.width, cellTexture.height), new Vector2(0.5f, 0.5f));

		cell.transform.SetParent(m_BoardHolder.transform);
		cell.transform.localScale = Vector3.one;
		cell.transform.localPosition = new Vector3(TEXTURE_SIZE * (y - m_Width / 2), -TEXTURE_SIZE * (x - m_Height / 2), 0f);

		return cell;
	}
}