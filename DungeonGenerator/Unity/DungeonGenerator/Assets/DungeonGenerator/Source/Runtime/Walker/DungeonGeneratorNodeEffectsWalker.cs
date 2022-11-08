using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using Unity.Mathematics;
using DungeonGenerator.Base;

namespace DungeonGenerator.Walker
{
    [System.Serializable]
    public class DungeonGeneratorNodeEffectWalkerGenerate : DungeonGeneratorNodeEffectBase
    {
        //Input Properties
        public int2 size;
        public int stepAmount;
        public int directionMaxSteps;
        public float directionChangeRate;

        //Consts
        List<int2> DIRECTIONS = new List<int2> { new int2(1, 0), new int2(0, -1), new int2(-1, 0), new int2(0, 1) };

        //Controller Properties
        private int2 controllerStepPosition;
        private int2 controllerDirection;
        private int controllerStepsSinceTurn;

        public override void OnEffect(DungeonGeneratorProfileBase profileBase)
        {
            //Get Profile
            var profile = (DungeonGeneratorProfileWalker)profileBase;
            if (!profile)
                return;

            OnEffectGenerate(profile);
        }

        public void OnEffectGenerate(DungeonGeneratorProfileWalker profile)
        {
            //Clamp Size
            size.x = math.clamp(size.x, 6, 999);
            size.y = math.clamp(size.y, 6, 999);

            //Init Data
            var dataRaw = new int[size.x, size.y];
            Texture2D dataTexture = new Texture2D(size.x, size.y);

            //Controller
            controllerStepPosition = new int2(size.x / 2, size.y / 2);
            controllerStepsSinceTurn = 0;
            controllerDirection = DIRECTIONS[0];

            //Steps
            for (int i = 0; i < stepAmount; i++)
            {
                //Add To History
                dataRaw[controllerStepPosition.x, controllerStepPosition.y] = 1;

                //Do Step If In Border
                if (IsInBounds(controllerStepPosition))
                    controllerStepPosition += controllerDirection;

                //Change Direction
                var rand = new System.Random();
                
                if (!IsInBounds(controllerStepPosition) || UnityEngine.Random.Range(0,2.0f) <= directionChangeRate || controllerStepsSinceTurn >= directionMaxSteps)
                {
                    controllerStepsSinceTurn = 0;

                    var T_DIRECTIONS = DIRECTIONS.ToList();
                    T_DIRECTIONS.Remove(controllerDirection);
                    controllerDirection = T_DIRECTIONS[UnityEngine.Random.Range(0,T_DIRECTIONS.Count)];

                    while (!IsInBounds(controllerStepPosition + controllerDirection))
                        controllerDirection = T_DIRECTIONS[UnityEngine.Random.Range(0,T_DIRECTIONS.Count)];
                }

            }


            for (int x = 0; x < size.x; x++)
                for (int y = 0; y < size.y; y++)
                    dataTexture.SetPixel(x, y, dataRaw[x, y] == 0 ? Color.black : Color.gray);
            dataTexture.Apply();

            //Apply Data
            DungeonGeneratorWalkerData.size = size;
            DungeonGeneratorWalkerData.dataRaw = dataRaw;
            DungeonGeneratorWalkerData.dataTexture = dataTexture;
        }

        public bool IsInBounds(int2 pos)
        {
            return pos.x > 2 && pos.x < size.x - 2 && pos.y > 2 && pos.y < size.y - 2;
        }
    }
}