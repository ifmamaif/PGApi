namespace PGApi
{
    public static class Random
    {
        private static readonly uint ms_Scale = 1103515245;
        private static readonly uint ms_Offset = 12345;
        private static readonly uint ms_Module = 2 << 31;
        private static uint ms_Seed = 691;

        public static uint Rand()
        {
            return ms_Seed = ((ms_Scale * ms_Seed) + ms_Offset) % ms_Module;
        }

        public static uint Rand(uint newseed)
        {
            return ms_Seed = ((ms_Scale * newseed) + ms_Offset) % ms_Module;
        }

        //public static uint Rand(ushort a,  ushort b)
        //{
        //    seed = ((uint)a << 16) | ((uint)b);
        //    return Rand(seed);
        //}

        public static uint Rand(uint a, uint b)
        {
            return ((Rand() % b) + a) % b;
        }

    }
}