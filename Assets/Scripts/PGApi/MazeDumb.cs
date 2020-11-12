using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class MazeDumb
{
    public static int[,] Generate(int width,int height)
    {
		int[,] result = new int[height, width];
		Vector2Int playerPos = new Vector2Int(0, 0);

		int randomNumber;
		System.Random randomGenerator = new System.Random();
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				randomNumber = randomGenerator.Next(1, 101); // de la 1 la 100
				result[i, j] = (randomNumber < 66) ? 1 : 0;
			}
		}
		result[playerPos.y, playerPos.x] = 1;

		return result;
	}


}
