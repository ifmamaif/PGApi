using System;
using System.Runtime.InteropServices;

namespace PGApi
{
    public static class PGApi
    {
        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GenereazăHartăPerlin2D(int width, int height, float scale, float offsetX, float offsetY);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GenerateMazeWorm(int worldSizeX, int worldSizeY, int numberOfRooms);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeleteMazeWorm(IntPtr buffer, int x, int y);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetValueMazeWorm(IntPtr buffer, int x, int y);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern int AleatoriuÎntre(int minInclusive, int maxExclusive);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool OutSidei(int x1, int y1, int x2, int y2);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RotunjireLaÎntregf(float f);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float InterpolareLiniarăf(float a0, float a1, float weight);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float Aleatoriu01f();

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float ZgomotulPerlin_Îmbunătățit1D(float x);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float ZgomotulPerlin_Îmbunătățit2D(float x, float y);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float ZgomotulPerlin_Îmbunătățit3D(float x, float y, float z);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float ZgomotulPerlin_Clasic1D(float x);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float ZgomotulPerlin_Clasic2D(float x, float y);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float ZgomotulPerlin_Clasic3D(float x, float y, float z);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float ZgomotulPerlin_Simplex2D(float x, float y);

        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float ZgomotulPerlin_Simplex3D(float x, float y, float z);
                
        [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
        public static extern float MișcareFracționatăBrowniană(float x, float y, float z, int numOctaves, float amplitude, float gain, float frequency, float lacunarity);
    }
}