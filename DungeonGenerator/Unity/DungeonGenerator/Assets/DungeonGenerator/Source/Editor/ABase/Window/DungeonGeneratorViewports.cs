using System;
using System.Collections.Generic;
using UnityEditor.Experimental.GraphView;
using UnityEngine;
using UnityEngine.UIElements;
using Unity.Mathematics;

namespace DungeonGenerator.Base
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Interface Viewport
    public class DungeonGeneratorViewportBase : GraphView
    {
        protected DungeonGeneratorWindowBase window;
        protected DungeonGeneratorSearchWindow searchWindow;
        protected GridBackground gridBackground;

        public int moduleIndex;
        public virtual void Update() { }
        public virtual void RenderViewport() { }

        public void SetupViewport(DungeonGeneratorWindowBase windowBase, float2 zoomMinMax, bool bUseGridBackground, bool bIsGraphView)
        {
            //Window
            window = windowBase;

            //Add Stylesheets
            styleSheets.Add(Resources.Load<StyleSheet>("DungeonGeneratorGraphStyle"));

            //Setup Grid Background
            if (bUseGridBackground)
            {
                var grid = new GridBackground();
                Insert(0, grid);
                grid.StretchToParentSize();
            }

            //Setup Zoom
            SetupZoom(zoomMinMax.x, zoomMinMax.y);

            //Is Graph View
            if (bIsGraphView)
                CreateGraphNodeSearchWindow(window);

            this.AddManipulator(new ContentDragger());
            this.AddManipulator(new SelectionDragger());
            this.AddManipulator(new RectangleSelector());
            this.AddManipulator(new FreehandSelector());

        }

        public void CreateNode(Type type, Vector2 position, string GUIDOverride, string Data)
        {
            if (type != null)
            {
                var node = (DungeonGeneratorNodeBase)Activator.CreateInstance(type);
                node.OnCreate();
                node.SetData(Data);
                node.OnRefresh();

                if (GUIDOverride != "")
                    node.GUID = GUIDOverride;

                //Add Node To Graph View
                node.SetPosition(new Rect(position.x, position.y, 500, 150));
                AddElement(node);
            }
        }

        public override List<Port> GetCompatiblePorts(Port startPort, NodeAdapter nodeAdapter)
        {
            var compatiblePorts = new List<Port>();
            var startPortView = startPort;

            ports.ForEach((port) =>
            {
                var portView = port;
                if (startPortView != portView && startPortView.node != portView.node)
                    compatiblePorts.Add(port);
            });

            return compatiblePorts;
        }

        protected void CreateGraphNodeSearchWindow(DungeonGeneratorWindowBase window)
        {
            searchWindow = ScriptableObject.CreateInstance<DungeonGeneratorSearchWindow>();
            searchWindow.Configure(window, this);
            nodeCreationRequest = context =>
                SearchWindow.Open(new SearchWindowContext(context.screenMousePosition), searchWindow);
        }

        public Group CreateCommentBlock(Rect rect)
        {
            var group = new Group
            {
                autoUpdateGeometry = true,
                title = "Comment"
            };
            AddElement(group);
            group.SetPosition(rect);
            return group;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Node Graph Viewport

    public class DungeonGeneratorNodeGraphViewport : DungeonGeneratorViewportBase
    {
        private ISelectable currentSelectable;
        private DungeonGeneratorNodeBase currenetSelectableAsDungeonGeneratorNode;

        public DungeonGeneratorNodeGraphViewport(DungeonGeneratorWindowBase window)
        {
            //Default Viewport With Graph View
            SetupViewport(window, new float2(ContentZoomer.DefaultMinScale, ContentZoomer.DefaultMaxScale), true, true);
        }

        public override void RenderViewport()
        {

        }

        public override void Update()
        {
            //Selection Changed
            if (selection.Count == 1 && selection[0] != currentSelectable)
            {
                currentSelectable = selection[0];
                if (currentSelectable.GetType().IsSubclassOf(typeof(DungeonGeneratorNodeBase)))
                {
                    currenetSelectableAsDungeonGeneratorNode = currentSelectable as DungeonGeneratorNodeBase;
                    window.OnNodeSelected(currenetSelectableAsDungeonGeneratorNode);
                }
                else
                {
                    currenetSelectableAsDungeonGeneratorNode = null;
                    window.OnNodeSelected(null);
                }

            }

            else if (selection.Count != 1)
            {
                currentSelectable = null;
                currenetSelectableAsDungeonGeneratorNode = null;
                window.OnNodeSelected(null);

            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //GUI Viewport
    public class DungeonGeneratorGUIViewport : DungeonGeneratorViewportBase
    {
        private IMGUIContainer iMGUIContainer;

        public DungeonGeneratorGUIViewport(DungeonGeneratorWindowBase window)
        {
            //Default Viewport
            SetupViewport(window, new float2(1,1), true, false);

            //GUI Viewport
            var a = Resources.Load<VisualTreeAsset>("DungeonGeneratorDataGraphElement");
            a.CloneTree(viewport);
            iMGUIContainer = viewport.Query<IMGUIContainer>("DataGraphIMGUIContainer");
            iMGUIContainer.onGUIHandler = RenderViewport;
        }

        public override void Update()
        {
        }

        public override void RenderViewport()
        {
            DungeonGeneratorProfileBase.GeLoadedProfileAsset().RenderViewport(DungeonGeneratorProfileBase.GeLoadedProfileAsset().modules[moduleIndex].uid, new float3(viewTransform.position.x, viewTransform.position.y, viewTransform.scale.magnitude));
        }
    }
}
