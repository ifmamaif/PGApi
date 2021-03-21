namespace PGApi
{
	public static class Perlin
	{
        public static double[,] Generate2DMapUnity(int width, int height, float scale, float offsetX, float offsetY)
		{
			double[,] noiseMap = new double[width, height];
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					double xCoord = (double)x / width * scale + offsetX;
					double yCoord = (double)y / height * scale + offsetY;
					noiseMap[x, y] = PGApi.PerlinNoise_Improved2D(xCoord, yCoord);
				}
			}
			return noiseMap;
		}
	}
}