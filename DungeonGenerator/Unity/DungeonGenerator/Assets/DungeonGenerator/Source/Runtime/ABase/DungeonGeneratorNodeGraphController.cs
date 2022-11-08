using System;
using UnityEngine;

namespace DungeonGenerator.Base
{
    public class DungeonGeneratorNodeGraphController
    {
        public void ExecuteNodeGraph(DungeonGeneratorProfileBase profile, DungeonGeneratorProfileModule module)
        {
            //Find Root
            DungeonGeneratorNodeData startNodeData = null;
            for (int i = 0; i < module.nodeDatas.Count; i++)
            {
                //Yes We Are A Transit Node Which Could Be A Root Node
                if (module.nodeDatas[i].nodeEffectTypeName == typeof(DungeonGeneratorNodeEffectBaseTransit).FullName)
                {
                    DungeonGeneratorNodeEffectBaseTransit data = new DungeonGeneratorNodeEffectBaseTransit();
                    JsonUtility.FromJsonOverwrite(module.nodeDatas[i].nodeEffectTypeData,data);

                    //We Are A Root Node So Set As Start Node Data
                    if (data.transitType == DungeonGeneratorNodeEffectBaseTransit.TransitType.Root)
                    {
                        startNodeData = module.nodeDatas[i];
                        break;
                    }
                }
            }
            
            //Execute Start Node
            ExecuteNode(profile, module,startNodeData);
        }

        public void ExecuteNode(DungeonGeneratorProfileBase profile, DungeonGeneratorProfileModule module,  DungeonGeneratorNodeData currentNodeData)
        {
            //Return If Current Node Data Is Null
            if (currentNodeData == null)
                return;

            //Do Node Effect
            DungeonGeneratorNodeEffectBase nodeEffectBase = (DungeonGeneratorNodeEffectBase)Activator.CreateInstance(Type.GetType(currentNodeData.nodeEffectTypeName));
            JsonUtility.FromJsonOverwrite(currentNodeData.nodeEffectTypeData,nodeEffectBase);
            nodeEffectBase.OnEffect(profile);

            //Find Next Node
            bool bFound = false;
            for (int i = 0; i < module.nodeLinkDatas.Count; i++)
            {
                if (module.nodeLinkDatas[i].outputNodeGUID == currentNodeData.GUID)
                {
                    for (int j = 0; j < module.nodeDatas.Count; j++)
                    {
                        if (module.nodeLinkDatas[i].inputNodeGUID == module.nodeDatas[j].GUID)
                        {
                            currentNodeData = module.nodeDatas[j];
                            bFound = true;
                        }
                    }
                }
            }

            //Nothing Found So Return
            if (!bFound)
            {
                currentNodeData = null;
                return;
            }

            //Found So Repeat
            ExecuteNode(profile,module,currentNodeData);
        }
    }
}
