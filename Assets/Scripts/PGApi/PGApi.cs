using System;
using System.Runtime.InteropServices;

namespace PGApi
{
    public static class PGApi
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

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RandBetween(int minInclusive, int maxExclusive);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool OutSidei(int x1, int y1, int x2, int y2);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RoundToInt(float f);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float Lerpf(float a0, float a1, float weight);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float Randf();

    }
}