using System.Collections.Generic;
using UnityEngine;
using Unity.Mathematics;

namespace DungeonGenerator.Base
{
    [System.Serializable]
    public abstract class DungeonGeneratorProfileBase : ScriptableObject
    {
        //Static
        public static DungeonGeneratorProfileBase loadedProfileAsset;
        public static DungeonGeneratorProfileBase GeLoadedProfileAsset()
        {
            return loadedProfileAsset;
        }

        public List<DungeonGeneratorProfileModule> modules;

        public abstract void Initialize();
        public abstract void Generate();
        public abstract void RenderViewport(string moduleUid, float3 viewportPosition);
    }

    [System.Serializable]
    public struct DungeonGeneratorProfileModule
    {
        public string uid;
        public string viewportName;
        public string viewportType;
        public List<DungeonGeneratorNodeData> nodeDatas;
        public List<DungeonGeneratorNodeLinkData> nodeLinkDatas;
    }
}
