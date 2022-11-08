using UnityEngine;

namespace DungeonGenerator.Base
{
    [System.Serializable]
    public class DungeonGeneratorNodeData
    {
        public string GUID;
        public string typeName;
        public string nodeEffectTypeName;
        public string nodeEffectTypeData;
        public Vector2 position;
    }

    [System.Serializable]
    public class DungeonGeneratorNodeLinkData
    {
        public string inputNodeGUID;
        public string outputNodeGUID;
    }

    public abstract class DungeonGeneratorNodeEffectBase
    {
        public abstract void OnEffect(DungeonGeneratorProfileBase profileBase);
    }

    [System.Serializable]
    public class DungeonGeneratorNodeEffectBaseTransit : DungeonGeneratorNodeEffectBase
    {
        public enum TransitType { Root, To, From };
        public TransitType transitType;

        public override void OnEffect(DungeonGeneratorProfileBase profileBase)
        {
            
        }
    }
}
