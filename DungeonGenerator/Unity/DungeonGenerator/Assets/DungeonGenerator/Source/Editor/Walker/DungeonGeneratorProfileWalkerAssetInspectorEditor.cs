using UnityEditor;
using UnityEngine;
using DungeonGenerator.Base;

namespace DungeonGenerator.Walker
{
    [CustomEditor(typeof(DungeonGeneratorProfileWalker))]
    public class DungeonGeneratorProfileWalkerAssetInspectorEditor : Editor
    {
        public override void OnInspectorGUI()
        {
            base.OnInspectorGUI();

            if (GUILayout.Button("Open"))
            {
                DungeonGeneratorProfileBase.loadedProfileAsset = (DungeonGeneratorProfileWalker)target;
                DungeonGeneratorWindowBase.Open();
            }
        }
    }
}
