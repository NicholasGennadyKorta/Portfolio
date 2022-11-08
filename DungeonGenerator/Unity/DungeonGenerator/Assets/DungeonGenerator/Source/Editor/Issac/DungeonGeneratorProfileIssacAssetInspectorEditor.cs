using UnityEditor;
using UnityEngine;
using DungeonGenerator.Base;

namespace DungeonGenerator.Issac
{
    [CustomEditor(typeof(DungeonGeneratorProfileIssac))]
    public class DungeonGeneratorProfileIssacAssetInspectorEditor : Editor
    {
        public override void OnInspectorGUI()
        {
            base.OnInspectorGUI();

            if (GUILayout.Button("Open"))
            {
                DungeonGeneratorProfileBase.loadedProfileAsset = (DungeonGeneratorProfileIssac)target;
                DungeonGeneratorWindowBase.Open();
            }
        }
    }
}
