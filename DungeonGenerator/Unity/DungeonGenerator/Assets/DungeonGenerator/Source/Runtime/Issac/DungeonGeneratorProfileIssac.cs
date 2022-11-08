using System.Collections.Generic;
using UnityEngine;
using Unity.Mathematics;
using DungeonGenerator.Base;

namespace DungeonGenerator.Issac
{
    public struct DungeonGeneratorProfileIssacRoom
    {
        public int3 cellPosition;
        public List<int3> neighborRoomCellPositions;
        public List<string> stamps;
    }

    [CreateAssetMenu(fileName = "Dungeon Generator Profile Issac", menuName = "Dungeon Generator/Profile Issac")]
    [System.Serializable]
    public class DungeonGeneratorProfileIssac : DungeonGeneratorProfileBase
    {
        //Data
        public List<DungeonGeneratorProfileIssacRoom> generatedRooms;
        public Dictionary<int3,int> Indexs;
        public bool bFailed;

        //GUI
        Texture2D pixelTexture;
        Texture2D startTexture;

        public override void Initialize()
        {
            if (modules == null || modules.Count == 0)
            {
                modules = new List<DungeonGeneratorProfileModule>();
                modules.Add(new DungeonGeneratorProfileModule() { uid = "mission", viewportName = "Mission Graph", viewportType = "nodeGraph" });
                modules.Add(new DungeonGeneratorProfileModule() { uid = "data", viewportName = "Data View", viewportType = "gui" });
            }
        }

        public override void Generate()
        {
            //Clear Data
            generatedRooms = new List<DungeonGeneratorProfileIssacRoom>();
            Indexs = new Dictionary<int3,int>();

            //Mission graph should be first added in Initalize
            DungeonGeneratorNodeGraphController controller = new DungeonGeneratorNodeGraphController();
            controller.ExecuteNodeGraph(this, modules[0]);
        }

        public int Convert3DArrayIndex(int3 value)
        {
            return Indexs[value];
        }

        public override void RenderViewport(string moduleUid, float3 viewportPosition)
        {
            if (moduleUid == "data")
                RenderViewportData(viewportPosition);
        }

        public void RenderViewportData(float3 viewportPosition)
        {
            DungeonGeneratorProfileIssac profile = (DungeonGeneratorProfileIssac)DungeonGeneratorProfileBase.loadedProfileAsset;

            //Render Key
            GUI.color = Color.black;
            GUI.Box(new Rect(10,Screen.height - 240,205,205),"");
            GUI.BeginGroup(new Rect(15,Screen.height - 235,200,200));
            GUI.color = Color.white;
            GUILayout.Label("Viewport Position:" + viewportPosition.x.ToString() + "," + viewportPosition.y.ToString());
            GUI.EndGroup();


            float renderScale = 60 + viewportPosition.z;
            var texture = (Texture2D)Resources.Load("Sprite_DungeonGenerator_Issac_Editor_Room");
            startTexture = new Texture2D(100,100);
            if (!texture || profile.generatedRooms == null) return;

            var pa = new float2();
            var pb = new float2();

            foreach (var room in profile.generatedRooms)
            {
                pa = GuiPosition(viewportPosition,room.cellPosition,renderScale);
                GUI.color = room.stamps.Contains("endRoom") ? Color.magenta : Color.white;
                GUI.Box(new Rect(pa.x,pa.y, renderScale, renderScale),texture);

                if (room.stamps.Contains("start"))
                {
                    GUI.color = Color.green;
                    GUI.DrawTexture(new Rect(pa.x + renderScale * 0.25f,pa.y + renderScale * 0.25f, renderScale * 0.5f, renderScale * 0.5f),startTexture);
                }

                foreach (var neighborRoomCellPosition in room.neighborRoomCellPositions)
                {
                    pa = GuiPositionCentered(viewportPosition,room.cellPosition,renderScale);
                    pb = GuiPositionCentered(viewportPosition,neighborRoomCellPosition,renderScale);
                    GuiDrawLine(pa,pb,Color.cyan,2);
                }
            }
        }
        

        public float2 GuiPosition(float3 viewportPosition,float3 position, float scale)
        {
            return new float2 ((position.x + viewportPosition.x * 0.01f) * scale, (position.y + viewportPosition.y * 0.01f) * scale);
        }

        public float2 GuiPositionCentered(float3 viewportPosition,float3 position, float scale)
        {
            return new float2 ((position.x + viewportPosition.x * 0.01f) * scale + (scale * 0.5f), (position.y + viewportPosition.y * 0.01f) * scale + (scale * 0.5f));
        }

        public void GuiDrawLine(Vector2 pointA, Vector2 pointB, Color color, float width)
        {
            // Save the current GUI matrix, since we're going to make changes to it.
            Matrix4x4 matrix = GUI.matrix;
            if (!pixelTexture) { pixelTexture = new Texture2D(1, 1); }
            Color savedColor = GUI.color;
            GUI.color = color;

            ////Angele
            float angle = Vector3.Angle(pointB - pointA, Vector2.right);
            if (pointA.y > pointB.y) { angle = -angle; }

            //Scale Rotate
            GUIUtility.ScaleAroundPivot(new Vector2((pointB - pointA).magnitude, width), new Vector2(pointA.x, pointA.y + 0.5f));
            GUIUtility.RotateAroundPivot(angle, pointA);
            GUI.DrawTexture(new Rect(pointA.x, pointA.y, 1, 1), pixelTexture);

            // We're done.  Restore the GUI matrix and GUI color to whatever they were before.
            GUI.matrix = matrix;
            GUI.color = savedColor;
        }
    }
}