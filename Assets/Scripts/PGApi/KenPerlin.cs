using System.Runtime.InteropServices;

namespace PGApi
{
	public static class Perlin
	{
		[DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
		private static extern double PerlinNoise2D(double x, double y);

		public static double[,] Generate2DMap(int width, int height, float scale, float offsetX, float offsetY)
		{
			double[,] noiseMap = new double[width, height];
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					double xCoord = (double)x / width * scale + offsetX;
					double yCoord = (double)y / height * scale + offsetY;
					noiseMap[x, y] = PerlinNoise2D(xCoord, yCoord);
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