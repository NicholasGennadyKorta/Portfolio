using System.Collections.Generic;
using Unity.Mathematics;
using DungeonGenerator.Base;

namespace DungeonGenerator.Issac
{
    [System.Serializable]
    public class DungeonGeneratorNodeEffectIssacGenerate : DungeonGeneratorNodeEffectBase
    {
        //Generation Properties
        public float2 roomAmountMinMax;
        public float2 endRoomAmountMinMax;

        //Flood Fill (Open And Closed Set)
        private List<int3> openSet;
        private List<int3> closedSet;

        public override void OnEffect(DungeonGeneratorProfileBase profileBase)
        {
            //Convert To And Make Sure Issac Profile
            var profile = (DungeonGeneratorProfileIssac)profileBase;
            if (profile == null) return;

            //Reset Generated Room Data | Create Structure For New Room
            profile.generatedRooms = new List<DungeonGeneratorProfileIssacRoom>();
            profile.Indexs = new Dictionary<int3,int>();

            //Flood Fill (Open And Closed Set)
            openSet = new List<int3>();
            closedSet = new List<int3>();

            //Create First Room | Add It To Open Set | Add Start Stamp
            AddRoom(profile,int3.zero,int3.zero);

            //Generate Rooms If There Are Open Amount And If We have Not Reached Max Amount
            while (openSet.Count != 0 && closedSet.Count < roomAmountMinMax.y)
            {
                //Get First Room In Open Set | Add It To Generated Rooms | Remove From OpenSet | Add To Closed Set
                var roomCellIndex = openSet[0];
                openSet.Remove(roomCellIndex);
                //closedSet.Add(roomCellIndex);

                //Expand North
                if (UnityEngine.Random.Range(0, 2) != 0 && !closedSet.Contains(roomCellIndex + new int3(0, 1, 0)))
                    AddRoom(profile,roomCellIndex,new int3(0,1,0));

                //Expand East
                if (UnityEngine.Random.Range(0, 2) != 0 && !closedSet.Contains(roomCellIndex + new int3(1, 0, 0)))
                    AddRoom(profile,roomCellIndex,new int3(1,0,0));

                //Expand South
                 if (UnityEngine.Random.Range(0, 2) != 0 && !closedSet.Contains(roomCellIndex + new int3(0, -1, 0)))
                    AddRoom(profile,roomCellIndex,new int3(0, -1,0));

                 //Expand West
                if (UnityEngine.Random.Range(0, 2) != 0 && !closedSet.Contains(roomCellIndex + new int3(-1, 0, 0)))
                    AddRoom(profile,roomCellIndex,new int3(-1,0,0));
            }

            //Not Enough Rooms :(
            if (closedSet.Count < roomAmountMinMax.x)
               profile.bFailed = true;

            //Get End Room
            foreach (var room in profile.generatedRooms)
            {
                if (room.neighborRoomCellPositions.Count == 1)
                    room.stamps.Add("endRoom");
            }
        }

        public void AddRoom(DungeonGeneratorProfileIssac profile, int3 currentCellPosition, int3 cellPositionOffset)
        {
            //Craete Room | Set Cell Position
            var newRoom = new DungeonGeneratorProfileIssacRoom();
            newRoom.neighborRoomCellPositions = new List<int3>();
            newRoom.stamps = new List<string>();
           
            //Link Rooms Together
            if (closedSet.Count != 0)
            {
                newRoom.neighborRoomCellPositions.Add(currentCellPosition);
                newRoom.cellPosition = currentCellPosition + cellPositionOffset;
                profile.generatedRooms[profile.Convert3DArrayIndex(currentCellPosition)].neighborRoomCellPositions.Add(newRoom.cellPosition);
            }
            else
            {
                cellPositionOffset = new int3(0,0,0);
                newRoom.stamps.Add("start");
            }

            //Add Room To Data
            profile.Indexs.Add(newRoom.cellPosition,profile.Indexs.Count);
            profile.generatedRooms.Add(newRoom);
            openSet.Add(newRoom.cellPosition);
            closedSet.Add(newRoom.cellPosition);
        }
    }
}