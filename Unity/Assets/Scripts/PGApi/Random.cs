namespace PGApi
{
    public static class Random
    {
        private static readonly uint scale = 1103515245;
        private static readonly uint offset = 12345;
        private static readonly uint module = 2 << 31;
        private static uint seed = 691;

        public static uint Rand()
        {
            return seed = ((scale * seed) + offset) % module;
        }

        public static uint Rand(uint newseed)
        {
            return seed = ((scale * newseed) + offset) % module;
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