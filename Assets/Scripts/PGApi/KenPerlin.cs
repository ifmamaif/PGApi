namespace PGApi
{
	public static class Perlin
	{
        public static float[,] Generate2DMapUnity(int width, int height, float scale, float offsetX, float offsetY)
		{
			float[,] noiseMap = new float[width, height];
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					float xCoord = (float)x / width * scale + offsetX;
					float yCoord = (float)y / height * scale + offsetY;
					noiseMap[x, y] = PGApi.ZgomotulPerlin_Îmbunătățit2D(xCoord, yCoord);
				}
			}
			return noiseMap;
		}
	}
}