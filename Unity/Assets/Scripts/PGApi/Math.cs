namespace PGApi
{
    public static class Math
    {
        public static readonly float EPSILON = 1e-7F;

        public static double Abs(double value)
        {
            return value > 0 ? value : -value;
        }

        public static float Floor(float f)
        {
            int integer_f = (int)f;
            if (Abs(f - integer_f) > EPSILON)
            {
                return f < 0 ? integer_f + 1 : integer_f;
            }
            return f;
        }

        public static int FloorToInt(float f)
        {
            int integer_f = (int)f;
            if (Abs(f - integer_f) > EPSILON)
            {
                return f < 0 ? integer_f + 1 : integer_f;
            }
            return integer_f;
        }

        public static int RoundToInt(float f)
        {
            const float HALF = 0.5f;
            if(f>=0)
            {
                return (int)(f + HALF);
            }
            return (int)-(Abs(f) + HALF);
        }

        // Function to linearly interpolate between a0 and a1
        // weight should be in the range [0.0, 1.0]
        public static float Lerp(float a0, float a1, float weight)
        {
            // this is slightly faster equivalent formula for : (1.0 - w)*a0 + w * a1
            return a0 + weight * (a1 - a0);
        }

        public static double Lerp(double a0, double a1, double weight)
        {
            // this is slightly faster equivalent formula for : (1.0 - w)*a0 + w * a1
            return a0 + weight * (a1 - a0);
        }

    }
}
