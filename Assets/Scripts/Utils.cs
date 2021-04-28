using UnityEngine;

namespace Assets.Scripts
{
    public static class Utils
    {
        public static Color HexToColor(string hex)
        {
            ColorUtility.TryParseHtmlString(hex, out Color myColor);
            return myColor;
        }
    }
}
