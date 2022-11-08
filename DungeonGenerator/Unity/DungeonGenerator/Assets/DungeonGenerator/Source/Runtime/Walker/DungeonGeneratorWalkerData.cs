using UnityEngine;
using Unity.Mathematics;

namespace DungeonGenerator.Walker
{
    public class DungeonGeneratorWalkerData
    {
        public static int[,] dataRaw;
        public static Texture2D dataTexture;
        public static int2 size;

        public static void Reset()
        {
            dataRaw = new int[0, 0];
            dataTexture = new Texture2D(0, 0);
            size = new int2(0, 0);
        }

        public static void RenderViewportData(float3 viewportPosition)
        {
            float renderScale = 3;
            if (dataTexture != null)
                GUI.DrawTexture(new Rect(viewportPosition.x * renderScale,viewportPosition.y * renderScale,dataTexture.width * renderScale,dataTexture.height * renderScale),dataTexture);
        }
    }
}