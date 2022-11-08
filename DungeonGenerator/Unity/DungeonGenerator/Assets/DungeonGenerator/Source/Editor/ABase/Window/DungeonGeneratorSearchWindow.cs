using System;
using System.Reflection;
using System.Collections.Generic;
using UnityEditor;
using UnityEditor.Experimental.GraphView;
using UnityEngine;
using UnityEngine.UIElements;

namespace DungeonGenerator.Base
{
    public class DungeonGeneratorSearchWindow : ScriptableObject, ISearchWindowProvider
    {
        private EditorWindow _window;
        private DungeonGeneratorViewportBase _graphView;

        private Texture2D _indentationIcon;

        private List<Type> NodeTypes = new List<Type>();

        public void Configure(EditorWindow window, DungeonGeneratorViewportBase graphView)
        {
            _window = window;
            _graphView = graphView;

            //Transparent 1px indentation icon as a hack
            _indentationIcon = new Texture2D(1, 1);
            _indentationIcon.SetPixel(0, 0, new Color(0, 0, 0, 0));
            _indentationIcon.Apply();

            //Get All Node Types
            foreach (Assembly a in AppDomain.CurrentDomain.GetAssemblies())
                foreach (Type t in a.GetTypes())
                    if (t.IsSubclassOf(typeof(DungeonGeneratorNodeBase)))
                    {
                        if (t.Namespace == GetType().Namespace || t.Namespace == DungeonGeneratorProfileBase.loadedProfileAsset.GetType().Namespace)
                            NodeTypes.Add(t);
                    }
        }

        public List<SearchTreeEntry> CreateSearchTree(SearchWindowContext context)
        {
            var tree = new List<SearchTreeEntry>();

            tree.Add(new SearchTreeGroupEntry(new GUIContent("Create Node"), 0));

            //Create Groups And Nodes
            var catagoriesUsed = new List<string>();
            foreach (Type i in NodeTypes)
            {
                DungeonGeneratorNodeBase instanceI = (DungeonGeneratorNodeBase)Activator.CreateInstance(i);

                if (!catagoriesUsed.Contains(instanceI.GetSearchGroup()) && instanceI.GetSearchGroup() != "")
                {
                    tree.Add(new SearchTreeGroupEntry(new GUIContent(instanceI.GetSearchGroup()), 1));

                    foreach (Type j in NodeTypes)
                    {
                        DungeonGeneratorNodeBase instanceJ = (DungeonGeneratorNodeBase)Activator.CreateInstance(j);

                        if (instanceI.GetSearchGroup() == instanceJ.GetSearchGroup())
                        {
                            var S = new SearchTreeEntry(new GUIContent(instanceJ.GetSearchTitle(), _indentationIcon));
                            S.level = 2;
                            S.userData = j;
                            tree.Add(S);
                        }
                    }

                    catagoriesUsed.Add(instanceI.GetSearchGroup());
                }
            }

            //Comment Block
            var CB = new SearchTreeEntry((new GUIContent("Comment Block", _indentationIcon)));
            CB.level = 1;
            CB.userData = new Group();
            //tree.Add(CB);


            return tree;
        }

        public bool OnSelectEntry(SearchTreeEntry SearchTreeEntry, SearchWindowContext context)
        {
            //Editor window-based mouse position
            var mousePosition = _window.rootVisualElement.ChangeCoordinatesTo(_window.rootVisualElement.parent,
                context.screenMousePosition - _window.position.position);
            var graphMousePosition = _graphView.contentViewContainer.WorldToLocal(mousePosition);

            if (SearchTreeEntry.userData.GetType() == typeof(Group))
            {
                var rect = new Rect(graphMousePosition, new Vector2(300, 300));
                _graphView.CreateCommentBlock(rect);
                return true;
            }

            //Node
            if (((Type)SearchTreeEntry.userData).IsSubclassOf(typeof(DungeonGeneratorNodeBase)))
            {
                _graphView.CreateNode((Type)SearchTreeEntry.userData, graphMousePosition,"","");
                return true;
            }

            return false;
        }
    }
}