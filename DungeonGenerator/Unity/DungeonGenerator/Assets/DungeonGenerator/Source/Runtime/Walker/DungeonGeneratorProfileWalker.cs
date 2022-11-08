using System.Collections.Generic;
using UnityEngine;
using Unity.Mathematics;
using DungeonGenerator.Base;

namespace DungeonGenerator.Walker
{
    [CreateAssetMenu(fileName = "Dungeon Generator Profile Walker", menuName = "Dungeon Generator/Profile Walker")]
    [System.Serializable]
    public class DungeonGeneratorProfileWalker : DungeonGeneratorProfileBase
    {
        public override void Initialize()
        {
            if (modules == null || modules.Count == 0)
            {
                modules = new List<DungeonGeneratorProfileModule>();
                modules.Add(new DungeonGeneratorProfileModule() { uid = "mission", viewportName = "Mission Graph", viewportType = "nodeGraph" });
                modules.Add(new DungeonGeneratorProfileModule() { uid = "data", viewportName = "Data View", viewportType = "gui" });
            }
        }

        public override void Generate()
        {
            DungeonGeneratorWalkerData.Reset();

            //Mission graph should be first added in Initalize
            DungeonGeneratorNodeGraphController controller = new DungeonGeneratorNodeGraphController();
            controller.ExecuteNodeGraph(this, modules[0]);
        }

        public override void RenderViewport(string moduleUid, float3 viewportPosition)
        {
            if (moduleUid == "data")
                DungeonGeneratorWalkerData.RenderViewportData(viewportPosition);
        }
    }
}