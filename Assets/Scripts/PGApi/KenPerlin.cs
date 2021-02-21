using System;
using System.Runtime.InteropServices;

namespace PGApi
{
	public static class Perlin
	{
        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr Generate2DMap(int width, int height, float scale, float offsetX, float offsetY);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr GenerateMazeWorm(int worldSizeX, int worldSizeY, int numberOfRooms);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		public static extern void DeleteMazeWorm(IntPtr buffer, int x, int y);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		public static extern int GetValueMazeWorm(IntPtr buffer, int x, int y);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		public static extern double PerlinNoise2DUnity(double x, double y);

        public static double[,] Generate2DMapUnity(int width, int height, float scale, float offsetX, float offsetY)
		{
			double[,] noiseMap = new double[width, height];
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					double xCoord = (double)x / width * scale + offsetX;
					double yCoord = (double)y / height * scale + offsetY;
					noiseMap[x, y] = PerlinNoise2DUnity(xCoord, yCoord);
				}
			}
			return noiseMap;
		}
	}

	public static class Utils
	{
		[DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		public static extern bool OutSidei(int x1, int y1, int x2, int y2);
	}

	public static class Math
	{
		[DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		public static extern int RoundToInt(float f);

		[DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		public static extern float Lerpf(float a0, float a1, float weight);
	}

	public static class Random
	{
		[DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		public static extern float Randf();
	}
}