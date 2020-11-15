using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public static class MazeDumb
{
	[DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
	private static extern int RandBetween(int minInclusive, int maxExclusive);

	public static int[,] Generate(int width, int height)
	{
		int[,] result = new int[height, width];
		Vector2Int playerPos = new Vector2Int(0, 0);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				result[i, j] = (RandBetween(1, 101) < 66) ? 1 : 0;
			}
		}
		result[playerPos.y, playerPos.x] = 1;

		return result;
	}
}