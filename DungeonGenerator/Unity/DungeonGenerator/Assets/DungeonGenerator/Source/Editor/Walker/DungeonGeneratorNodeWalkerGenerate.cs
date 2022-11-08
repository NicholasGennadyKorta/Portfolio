using UnityEditor;
using DungeonGenerator.Base;

namespace DungeonGenerator.Walker
{
    public class DungeonGeneratorNodeWalkerGenerate : DungeonGeneratorNodeBase
    {
        private DungeonGeneratorNodeEffectWalkerGenerate data;

        public override string GetTitle()
        {
            return "Walker Generate";
        }

        public override string GetSearchTitle()
        {
            return "Walker Generate";
        }

        public override string GetSearchGroup()
        {
            return "Walker";
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
            data = new DungeonGeneratorNodeEffectWalkerGenerate();
            nodeEffectTypeName = data.GetType().FullName;
            CreatePorts(true, true);
        }

        public override void OnGUI()
        {
            EditorGUILayout.LabelField("Main",EditorStyles.boldLabel);
            data.size.x = EditorGUILayout.IntField("Size x:",data.size.x);
            data.size.y = EditorGUILayout.IntField("Size y:",data.size.y);
            data.stepAmount = EditorGUILayout.IntField("Step Amoumt:",data.stepAmount);

            EditorGUILayout.Space();
            EditorGUILayout.LabelField("Direction",EditorStyles.boldLabel);
            data.directionMaxSteps = EditorGUILayout.IntField("Direction Max Steps:",data.directionMaxSteps);
            data.directionChangeRate = EditorGUILayout.FloatField("Direction Change Rate:",data.directionChangeRate);

            nodeEffectTypeData = EditorJsonUtility.ToJson(data);
        }
    }
}
