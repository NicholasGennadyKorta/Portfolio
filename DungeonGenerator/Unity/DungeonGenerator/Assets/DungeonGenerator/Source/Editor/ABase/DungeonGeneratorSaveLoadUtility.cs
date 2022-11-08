using System;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEditor.Experimental.GraphView;
using UnityEngine;
using UnityEngine.UIElements;

namespace DungeonGenerator.Base
{
    public class DungeonGeneratorSaveLoadUtility
    {
        private DungeonGeneratorViewportBase graphView;
        private int moduleIndex;

        private List<Edge> edges => graphView.edges.ToList();
        private List<DungeonGeneratorNodeBase> nodes => graphView.nodes.ToList().Cast<DungeonGeneratorNodeBase>().ToList();

        public static DungeonGeneratorSaveLoadUtility GetInstance(DungeonGeneratorViewportBase targetGraphView, int targetModuleIndex)
        {
            return new DungeonGeneratorSaveLoadUtility
            {
                graphView = targetGraphView,
                moduleIndex = targetModuleIndex
            };
        }

        public void SaveGraph()
        {
            //Get Asset | Exit If Not Valid
            var asset = DungeonGeneratorProfileBase.GeLoadedProfileAsset();
            if (asset == null) return;
            
            //Get Module | Start With Clean Data
            var module = asset.modules[moduleIndex];
            module.nodeLinkDatas = new List<DungeonGeneratorNodeLinkData>();
            module.nodeDatas = new List<DungeonGeneratorNodeData>();

            //Port Connections
            {
                var connectedPorts = edges.ToArray(); //.Where(x => x.input.node != null).ToArray();

                for (var i = 0; i < connectedPorts.Length; i++)
                {
                    var outputNode = connectedPorts[i].output.node as DungeonGeneratorNodeBase;
                    var inputNode = connectedPorts[i].input.node as DungeonGeneratorNodeBase;

                    module.nodeLinkDatas.Add(new DungeonGeneratorNodeLinkData
                    {
                        outputNodeGUID = outputNode.GUID,
                        inputNodeGUID = inputNode.GUID,
                    });
                }
            }

            //Nodes
            foreach (var node in nodes)
            {
                var z = new DungeonGeneratorNodeData
                {
                    GUID = node.GUID,
                    typeName = node.GetType().FullName,
                    position = node.GetPosition().position,
                    nodeEffectTypeName = node.nodeEffectTypeName,
                    nodeEffectTypeData = node.GetData()
                };

                module.nodeDatas.Add(z);
            }

            asset.modules[moduleIndex] = module;
            AssetDatabase.SaveAssets();
        }

        public void LoadGraph()
        {
            var asset = DungeonGeneratorProfileBase.GeLoadedProfileAsset();
            if (asset == null) return;
                
            //Clear Graph
            {
                foreach (var node in nodes)
                    graphView.RemoveElement(node);

                foreach (var edge in edges)
                    graphView.RemoveElement(edge);

            }

            var module = asset.modules[moduleIndex];
            if (module.nodeDatas == null || module.nodeLinkDatas == null) return;


            //Nodes
            foreach (var nodeData in module.nodeDatas)
            {
                graphView.CreateNode(Type.GetType(nodeData.typeName), nodeData.position, nodeData.GUID, nodeData.nodeEffectTypeData);
            }

            //Port Connections
            for (int i = 0; i < nodes.Count(); i++)
            {
                foreach (var connection in module.nodeLinkDatas)
                {
                    if (connection.outputNodeGUID == nodes[i].GUID)
                    {
                        var targetNodeGUID = connection.inputNodeGUID;
                        var targetNode = nodes.First(x => x.GUID == targetNodeGUID);
                        LinkNodesTogether(nodes[i].outputContainer[0].Q<Port>(), (Port)targetNode.inputContainer[0]);
                    }
                }
            }
        }

        private void LinkNodesTogether(Port outputSocket, Port inputSocket)
        {
            var tempEdge = new Edge()
            {
                output = outputSocket,
                input = inputSocket
            };
            tempEdge?.input.Connect(tempEdge);
            tempEdge?.output.Connect(tempEdge);
            graphView.Add(tempEdge);
        }
    }
}
