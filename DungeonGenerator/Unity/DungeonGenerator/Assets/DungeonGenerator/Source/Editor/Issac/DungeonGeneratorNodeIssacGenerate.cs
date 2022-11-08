using UnityEditor;
using DungeonGenerator.Base;

namespace DungeonGenerator.Issac
{
    public class DungeonGeneratorNodeIssacGenerate : DungeonGeneratorNodeBase
    {
        private DungeonGeneratorNodeEffectIssacGenerate data;

        public override string GetTitle()
        {
            return "Issac Generate";
        }

        public override string GetSearchTitle()
        {
            return "Issac Generate";
        }

        public override string GetSearchGroup()
        {
            return "Issac";
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
            data = new DungeonGeneratorNodeEffectIssacGenerate();
            nodeEffectTypeName = data.GetType().FullName;
            CreatePorts(true, true);
        }

        public override void OnGUI()
        {
            data.roomAmountMinMax = EditorGUILayout.Vector2Field("Room Amount Min Max:",data.roomAmountMinMax);
            data.endRoomAmountMinMax = EditorGUILayout.Vector2Field("End Room Amount Min Max:",data.endRoomAmountMinMax);
            nodeEffectTypeData = EditorJsonUtility.ToJson(data);
        }
    }
}
