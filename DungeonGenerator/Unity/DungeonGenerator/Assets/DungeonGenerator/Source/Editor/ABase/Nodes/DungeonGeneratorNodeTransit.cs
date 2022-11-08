using UnityEditor;

namespace DungeonGenerator.Base
{
    public class DungeonGeneratorNodeTransit : DungeonGeneratorNodeBase
    {
        DungeonGeneratorNodeEffectBaseTransit data;

        public override string GetTitle()
        {
            if (data == null)
                return "Transit";
            else
                return "Transit : " + data.transitType.ToString();
        }

        public override string GetSearchTitle()
        {
            return "Transit";
        }

        public override string GetSearchGroup()
        {
            return "Base";
        }

        public override string GetData()
        {
            return EditorJsonUtility.ToJson(data);
        }

        public override void SetData(string dataAsString)
        {
             EditorJsonUtility.FromJsonOverwrite(dataAsString,data);
        }

        public override void OnCreate()
        {
            base.OnCreate();
            data = new DungeonGeneratorNodeEffectBaseTransit();
            nodeEffectTypeName = data.GetType().FullName;
            CreatePorts(false, true);
        }

    
        public override void OnGUI()
        {
            data.transitType = (DungeonGeneratorNodeEffectBaseTransit.TransitType)EditorGUILayout.EnumPopup("Transit Type:", data.transitType);
            OnRefresh();
        }
    }
}