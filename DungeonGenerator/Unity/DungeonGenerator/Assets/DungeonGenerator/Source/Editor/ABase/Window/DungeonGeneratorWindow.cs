using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;

namespace DungeonGenerator.Base
{
    public class DungeonGeneratorWindowBase : EditorWindow
    {
        private List<DungeonGeneratorViewportBase> viewports = new List<DungeonGeneratorViewportBase>();
        private int viewportIndex;
        private bool bIsUILoaded;

        public static void Open()
        {
            var window = (DungeonGeneratorWindowBase)EditorWindow.GetWindow(typeof(DungeonGeneratorWindowBase));
            window.titleContent = new GUIContent("DungeonGenerator");
            window.Show();

            window.OnEnable();
        }

        public void OnEnable()
        {
            DungeonGeneratorProfileBase.loadedProfileAsset.Initialize();

            GenerateViewports();
            GenerateToolbar();
            GenerateViewportButtons();
            GenerateInspector();
            RequestLoadSave(false);
            SetViewportItem(0);

            bIsUILoaded = true;
        }

        public void OnDisable()
        {
            //Remove Viewports If Already Craeted
            for (int i = 0; i < viewports.Count; i++)
                rootVisualElement.Remove((VisualElement)viewports[i]);
            viewports.Clear();

            bIsUILoaded = false;
        }

        public void SetViewportItem(int index)
        {
            int previousViewportIndex = viewportIndex;
            viewportIndex = index;
            ((DungeonGeneratorViewportBase)viewports[viewportIndex]).PlaceInFront(viewports[previousViewportIndex]);
            OnNodeSelected(null);
        }

        public void RequestLoadSave(bool bDoSave)
        {
            for (int i = 0; i < viewports.Count; i++)
            {
                var utility = DungeonGeneratorSaveLoadUtility.GetInstance(viewports[i], i);

                if (bDoSave)
                    utility.SaveGraph();
                else
                    utility.LoadGraph();
            }

            EditorUtility.SetDirty(DungeonGeneratorProfileBase.loadedProfileAsset);
        }

        public void OnNodeSelected(DungeonGeneratorNodeBase node)
        {
            //File Name Label | InspectorGUI
            Label l = rootVisualElement.Query<Label>("NodeNameLabel");
            IMGUIContainer r = rootVisualElement.Query<IMGUIContainer>("InspectorIMGUIContainer");
            if (r == null) 
                return;

            if (node != null)
            {
                l.text = node.title;
                r.onGUIHandler = node.OnGUI;
            }
            else
            {
                l.text = "Inspector";
                r.onGUIHandler = null;
            }
        }

        private void GenerateViewports()
        {
            //Remove Viewports If Already Craeted
            for (int i = 0; i < viewports.Count; i++)
                rootVisualElement.Remove((VisualElement)viewports[i]);
            viewports.Clear();

            var modules = DungeonGeneratorProfileBase.loadedProfileAsset.modules;
            for (int i = 0; i < modules.Count && i < 5; i++)
            {
                if (modules[i].viewportType == "nodeGraph")
                {
                    var e = new DungeonGeneratorNodeGraphViewport(this);
                    e.StretchToParentSize();
                    e.moduleIndex = i;
                    rootVisualElement.Add(e);
                    viewports.Add(e);
                }

                if (modules[i].viewportType == "gui")
                {
                    var e = new DungeonGeneratorGUIViewport(this);
                    e.StretchToParentSize();
                    e.moduleIndex = i;
                    rootVisualElement.Add(e);
                    viewports.Add(e);
                }
            }
        }

        private void GenerateToolbar()
        {
            if (!bIsUILoaded)
                return;

            //Add Toolbar
            var a = Resources.Load<VisualTreeAsset>("DungeonGeneratorToolbarElement");
            a.CloneTree(rootVisualElement);

            //Save Button
            Button s = rootVisualElement.Query<Button>("SaveButton");
            s.clicked += () => RequestLoadSave(true);

            //Load
            Button l = rootVisualElement.Query<Button>("LoadButton");
            l.clicked += () => RequestLoadSave(false);

            //Generate
            Button b = rootVisualElement.Query<Button>("GenerateButton");
            b.clicked += () => RequestLoadSave(true);
            b.clicked += () => DungeonGeneratorProfileBase.GeLoadedProfileAsset().Generate();
        }

        private void GenerateViewportButtons()
        {
            if (!bIsUILoaded)
                return;

            //Add Viewport Buttons
            var modules = DungeonGeneratorProfileBase.loadedProfileAsset.modules;
            for (int i = 0; i < modules.Count && i < 5; i++)
            {
                int index = 0;
                int amount = 0;
                var b = Resources.Load<VisualTreeAsset>("DungeonGeneratorViewportButtonElement");
                b.CloneTree(rootVisualElement, out index, out amount);

                Button button = (Button)rootVisualElement.ElementAt(index);
                if (button != null)
                {
                    switch (i)
                    {
                        case 0: button.clicked += () => SetViewportItem(0); break;
                        case 1: button.clicked += () => SetViewportItem(1); break;
                        case 2: button.clicked += () => SetViewportItem(2); break;
                        case 3: button.clicked += () => SetViewportItem(3); break;
                        case 4: button.clicked += () => SetViewportItem(4); break;
                    }

                    button.text = modules[i].viewportName;
                }
            }
        }

        private void GenerateInspector()
        {
            if (!bIsUILoaded)
                return;

            //Add Inspector
            var a = Resources.Load<VisualTreeAsset>("DungeonGeneratorInspectorElement");
            a.CloneTree(rootVisualElement);

            //File Name Label
            Label l = rootVisualElement.Query<Label>("FileNameLabel");
            if (l != null)
                l.text = DungeonGeneratorProfileBase.GeLoadedProfileAsset().name;
        }

        private void Update()
        {
            viewports[viewportIndex].Update();
        }

        private void OnGUI()
        {
            //Make Sure If Asset Is Deleted We Close Editor
            if (DungeonGeneratorProfileBase.GeLoadedProfileAsset() == null)
                this.Close();
        }
    }
}