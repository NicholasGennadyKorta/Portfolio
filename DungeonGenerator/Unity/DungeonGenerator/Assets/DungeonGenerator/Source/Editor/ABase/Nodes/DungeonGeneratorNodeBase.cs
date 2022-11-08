using System;
using UnityEditor.Experimental.GraphView;

namespace DungeonGenerator.Base
{
    public class DungeonGeneratorNodeBase : Node
    {
        public string GUID;
        public string nodeEffectTypeName;
        public string nodeEffectTypeData;

        public virtual string GetTitle() { return ""; }
        public virtual string GetSearchTitle() { return ""; }
        public virtual string GetSearchGroup() { return ""; }
        public virtual string GetData() {return "";}
        public virtual void SetData(string dataAsString) { }
        public virtual void OnGUI() { }

        public virtual void OnCreate() 
        {
            title = GetTitle();
            GUID = Guid.NewGuid().ToString();
        }


        public void CreatePorts(bool bHasInput, bool bHasOutput)
        {
            //Create Node Output Port
            if (bHasInput)
            {
                var inputPort = CreatePort(this, Direction.Input);
                inputPort.portName = "";
                inputContainer.Add(inputPort);
            }

            //Create Node Output Port
            if (bHasOutput)
            {
                var outputPort = CreatePort(this, Direction.Output);
                outputPort.portName = "";
                outputContainer.Add(outputPort);
            }
        }

        public void OnRefresh()
        {
            title = GetTitle();
            RefreshExpandedState();
            RefreshPorts();
        }

        public Port CreatePort(DungeonGeneratorNodeBase node, Direction direction, Port.Capacity capacity = Port.Capacity.Multi)
        {
            return InstantiatePort(Orientation.Horizontal, direction, capacity, typeof(float));
        }
    }
}
