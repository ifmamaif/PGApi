using PGApi;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public static class MazeWhorm
{
    public class Room
    {
        public bool doorTop, doorBot, doorLeft, doorRight;
    }

    private static Vector2Int ms_GridSize = new Vector2Int();
    private static List<Vector2Int> ms_TakenPositions = null;
    private const int MAX_ITERATIONS = 100;
    private const int HALF_ITERATION = MAX_ITERATIONS / 2;

    public static Room[,] Generate(Vector2Int worldSize, int numberOfRooms)
    {
        numberOfRooms = CheckInputData(worldSize, numberOfRooms);

        var result = CreateRooms(numberOfRooms);//lays out the actual map
        result = SetRoomDoors(result);          //assigns the doors where rooms would connect
        return result;
    }

    private static int CheckInputData(Vector2Int worldSize, int numberOfRooms)
    {
        int maximFit = worldSize.x * 2 * worldSize.y * 2;
        if (numberOfRooms >= maximFit)
        { // make sure we don't try to make more rooms than can fit in our grid
            numberOfRooms = PGApi.Math.RoundToInt(maximFit);
        }

        //note: these are half-extents
        ms_GridSize = worldSize;

        return numberOfRooms;
    }

    private static Room[,] CreateRooms(int numberOfRooms)
    {
        //setup
        Room[,] m_Rooms = new Room[ms_GridSize.x * 2, ms_GridSize.y * 2];
        m_Rooms[ms_GridSize.x, ms_GridSize.y] = new Room();
        ms_TakenPositions = new List<Vector2Int>() { Vector2Int.zero };

        //magic numbers
        const float RANDOM_COMPARE_START = 0.2f;
        const float RANDOM_COMPARE_END = 0.01f;
        //add rooms
        for (int i = 0; i < numberOfRooms - 1; i++)
        {
            float randomPerc = i / ((float)numberOfRooms - 1);
            float randomCompare = PGApi.Math.Lerpf(RANDOM_COMPARE_START, RANDOM_COMPARE_END, randomPerc);
            //grab new position
            Vector2Int checkPos = NewPosition();
            //test new position
            if (NumberOfNeighbors(checkPos, ms_TakenPositions) > 1 && UnityEngine.Random.value > randomCompare)
            {
                int iterations = 0;
                do
                {
                    checkPos = SelectiveNewPosition();
                    iterations++;
                } while (NumberOfNeighbors(checkPos, ms_TakenPositions) > 1 && iterations < MAX_ITERATIONS);
                if (iterations >= HALF_ITERATION)
                {
                    // print("error: could not create with fewer neighbors than : " + NumberOfNeighbors(checkPos, takenPositions));
                }
            }
            //finalize position
            m_Rooms[checkPos.x + ms_GridSize.x, checkPos.y + ms_GridSize.y] = new Room();
            ms_TakenPositions.Insert(0, checkPos);
        }

        return m_Rooms;
    }

    // method differs from the above in the two commented ways
    private static Vector2Int SelectiveNewPosition()
    {
        int inc;
        Vector2Int checkingPos;
        do
        {
            inc = 0;
            int index;
            do
            {
                //instead of getting a room to find an adject empty space, we start with one that only 
                //as one neighbor. This will make it more likely that it returns a room that branches out
                index = Mathf.RoundToInt(UnityEngine.Random.value * (ms_TakenPositions.Count - 1));
                inc++;
            } while (NumberOfNeighbors(ms_TakenPositions[index], ms_TakenPositions) > 1 && inc < MAX_ITERATIONS);

            checkingPos = GetNewPosition(index);

        } while (ms_TakenPositions.Contains(checkingPos) ||
                 PGApi.Utils.OutSidei(checkingPos.x, checkingPos.y, ms_GridSize.x, ms_GridSize.y)
                );

        if (inc >= MAX_ITERATIONS)
        { // break loop if it takes too long: this loop isn't garuanteed to find solution, which is fine for this
          // print("Error: could not find position with only one neighbor");
        }

        return checkingPos;
    }

    private static Vector2Int GetNewPosition(int index)
    {
        //capture its x, y position
        int x = ms_TakenPositions[index].x;
        int y = ms_TakenPositions[index].y;

        int upDown = UnityEngine.Random.value < 0.5f ? 1 : 0;       //randomly pick wether to look on hor or vert axis
        int positive = UnityEngine.Random.value < 0.5f ? 1 : -1;    //pick whether to be positive or negative on that axis

        //find the position based on the above bools
        return new Vector2Int(x + (1 - upDown) * positive, y + (upDown * positive));
    }


    private static int NumberOfNeighbors(Vector2Int checkingPos, List<Vector2Int> usedPositions)
    {
        int ret = 0; // start at zero, add 1 for each side there is already a room
        ret += usedPositions.Contains(checkingPos + Vector2Int.right) ? 1 : 0;
        ret += usedPositions.Contains(checkingPos + Vector2Int.left) ? 1 : 0;
        ret += usedPositions.Contains(checkingPos + Vector2Int.up) ? 1 : 0;
        ret += usedPositions.Contains(checkingPos + Vector2Int.down) ? 1 : 0;

        return ret;
    }

    private static Room[,] SetRoomDoors(Room[,] rooms)
    {
        for (int x = 0; x < ms_GridSize.x * 2; x++)
        {
            for (int y = 0; y < ms_GridSize.y * 2; y++)
            {
                if (rooms[x, y] == null)
                {
                    continue;
                }

                rooms[x, y].doorBot = y - 1 >= 0 && rooms[x, y - 1] != null;
                rooms[x, y].doorTop = y + 1 < ms_GridSize.y * 2 && rooms[x, y + 1] != null;
                rooms[x, y].doorLeft = x - 1 >= 0 && rooms[x - 1, y] != null;
                rooms[x, y].doorRight = x + 1 < ms_GridSize.x * 2 && rooms[x + 1, y] != null;
            }
        }

        return rooms;
    }

    private static Vector2Int NewPosition()
    {
        Vector2Int checkingPos;
        int index;
        do
        {
            index = PGApi.Math.RoundToInt(UnityEngine.Random.value * (ms_TakenPositions.Count - 1)); // pick a random room
            checkingPos = GetNewPosition(index);
        } while (ms_TakenPositions.Contains(checkingPos) ||
                 PGApi.Utils.OutSidei(checkingPos.x, checkingPos.y, ms_GridSize.x, ms_GridSize.y)
                 ); //make sure the position is valid

        return checkingPos;
    }
}