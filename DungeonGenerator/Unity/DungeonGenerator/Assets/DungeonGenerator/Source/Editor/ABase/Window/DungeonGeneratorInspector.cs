using UnityEngine;
using UnityEngine.UIElements;

namespace DungeonGenerator.Base
{
    public class DungeonGeneratorInspector : VisualElement
    {
        public DungeonGeneratorInspector(VisualElement root)
        {
            //Add Inspector
            var a = Resources.Load<VisualTreeAsset>("DungeonGeneratorInspectorElement");
            a.CloneTree(root);
           
        }

        public string stringAttr { get; set; }
        public float floatAttr { get; set; }
        public double doubleAttr { get; set; }
        public int intAttr { get; set; }
        public long longAttr { get; set; }
        public bool boolAttr { get; set; }
        public Color colorAttr { get; set; }

    }
}