using System.Collections.Generic;
using UnityEngine;

public class LevelGeneration : GenericBehaviour
{
    private GameObject level = null;
    private Vector2 worldSize = new Vector2(4, 4);
    private Room[,] rooms = null;
    private List<Vector2> takenPositions = null;
    private int gridSizeX, gridSizeY, numberOfRooms = 32;
    public GameObject roomWhiteObj = null;
    
    public override void Start()
    {
        roomWhiteObj = (GameObject)Resources.Load("Prefabs/MapSprite", typeof(GameObject));
        Generate();
    }

    public override void Generate()
    {
        Destroy(level);
        level = new GameObject("Level");
        level.transform.SetParent(gameObject.transform);

        if(roomWhiteObj == null)
            roomWhiteObj = (UnityEngine.GameObject)Resources.Load("Prefabs/MapSprite", typeof(GameObject));

        if (numberOfRooms >= worldSize.x * 2 * (worldSize.y * 2))
        { // make sure we dont try to make more rooms than can fit in our grid
            numberOfRooms = Mathf.RoundToInt(worldSize.x * 2 * (worldSize.y * 2));
        }
        gridSizeX = Mathf.RoundToInt(worldSize.x); //note: these are half-extents
        gridSizeY = Mathf.RoundToInt(worldSize.y);
        CreateRooms(); //lays out the actual map
        SetRoomDoors(); //assigns the doors where rooms would connect
        DrawMap(); //instantiates objects to make up a map
    }

    private void CreateRooms()
    {
        //setup
        rooms = new Room[gridSizeX * 2, gridSizeY * 2];
        rooms[gridSizeX, gridSizeY] = new Room(Vector2.zero, 1);
        takenPositions = new List<Vector2>();
        takenPositions.Insert(0, Vector2.zero);

        //magic numbers
        float randomCompareStart = 0.2f, randomCompareEnd = 0.01f;
        //add rooms
        for (int i = 0; i < numberOfRooms - 1; i++)
        {
            float randomPerc = i / ((float)numberOfRooms - 1);
            float randomCompare = Mathf.Lerp(randomCompareStart, randomCompareEnd, randomPerc);
            //grab new position
            Vector2 checkPos = NewPosition();
            //test new position
            if (NumberOfNeighbors(checkPos, takenPositions) > 1 && Random.value > randomCompare)
            {
                int iterations = 0;
                do
                {
                    checkPos = SelectiveNewPosition();
                    iterations++;
                } while (NumberOfNeighbors(checkPos, takenPositions) > 1 && iterations < 100);
                if (iterations >= 50)
                {
                   // print("error: could not create with fewer neighbors than : " + NumberOfNeighbors(checkPos, takenPositions));
                }
            }
            //finalize position
            rooms[(int)checkPos.x + gridSizeX, (int)checkPos.y + gridSizeY] = new Room(checkPos, 0);
            takenPositions.Insert(0, checkPos);
        }
    }

    private Vector2 NewPosition()
    {
        Vector2 checkingPos;
        int index;
        do
        {
            index = Mathf.RoundToInt(Random.value * (takenPositions.Count - 1)); // pick a random room

            checkingPos = GetNewPosition(index);

        } while (takenPositions.Contains(checkingPos)
            || checkingPos.x >= gridSizeX
            || checkingPos.x < -gridSizeX
            || checkingPos.y >= gridSizeY
            || checkingPos.y < -gridSizeY
        ); //make sure the position is valid
        return checkingPos;
    }

    // method differs from the above in the two commented ways
    private Vector2 SelectiveNewPosition()
    { 
        int inc;
        Vector2 checkingPos;
        do
        {
            inc = 0;
            int index;
            do
            {
                //instead of getting a room to find an adject empty space, we start with one that only 
                //as one neighbor. This will make it more likely that it returns a room that branches out
                index = Mathf.RoundToInt(Random.value * (takenPositions.Count - 1));
                inc++;
            } while (NumberOfNeighbors(takenPositions[index], takenPositions) > 1 && inc < 100);

            checkingPos = GetNewPosition(index);

        } while (takenPositions.Contains(checkingPos)
            || checkingPos.x >= gridSizeX
            || checkingPos.x < -gridSizeX
            || checkingPos.y >= gridSizeY
            || checkingPos.y < -gridSizeY
        );

        if (inc >= 100)
        { // break loop if it takes too long: this loop isnt garuanteed to find solution, which is fine for this
            print("Error: could not find position with only one neighbor");
        }
        return checkingPos;
    }

    private Vector2 GetNewPosition(int index)
    {
        //capture its x, y position
        int x = (int)takenPositions[index].x;
        int y = (int)takenPositions[index].y;
        bool UpDown = Random.value < 0.5f;      //randomly pick wether to look on hor or vert axis
        bool positive = Random.value < 0.5f;    //pick whether to be positive or negative on that axis

        //find the position bnased on the above bools
        if (UpDown)
        {
            y = positive ? y + 1 : y - 1;
        }
        else
        {
            x = positive ? x + 1 : x - 1;
        }

        return new Vector2(x, y);
    }

    private int NumberOfNeighbors(Vector2 checkingPos, List<Vector2> usedPositions)
    {
        int ret = 0; // start at zero, add 1 for each side there is already a room
        if (usedPositions.Contains(checkingPos + Vector2.right))
        { //using Vector.[direction] as short hands, for simplicity
            ret++;
        }
        if (usedPositions.Contains(checkingPos + Vector2.left))
        {
            ret++;
        }
        if (usedPositions.Contains(checkingPos + Vector2.up))
        {
            ret++;
        }
        if (usedPositions.Contains(checkingPos + Vector2.down))
        {
            ret++;
        }
        return ret;
    }

    private void DrawMap()
    {
        foreach (Room room in rooms)
        {
            if (room == null)
            {
                continue; //skip where there is no room
            }
            Vector2 drawPos = room.gridPos;
            drawPos.x *= 16;//aspect ratio of map sprite
            drawPos.y *= 8;
            //create map obj and assign its variables
            GameObject cell = Instantiate(roomWhiteObj, drawPos, Quaternion.identity);
            cell.transform.SetParent(level.transform);
            MapSpriteSelector mapper = cell.GetComponent<MapSpriteSelector>();
            mapper.type = room.type;
            mapper.up = room.doorTop;
            mapper.down = room.doorBot;
            mapper.right = room.doorRight;
            mapper.left = room.doorLeft;
        }
    }

    private void SetRoomDoors()
    {
        for (int x = 0; x < gridSizeX * 2; x++)
        {
            for (int y = 0; y < gridSizeY * 2; y++)
            {
                if (rooms[x, y] == null)
                {
                    continue;
                }

                rooms[x, y].doorBot = y - 1 >= 0 && rooms[x, y - 1] != null;
                rooms[x, y].doorTop = y + 1 < gridSizeY * 2 && rooms[x, y + 1] != null;
                rooms[x, y].doorLeft = x - 1 >= 0 && rooms[x - 1, y] != null;
                rooms[x, y].doorRight = x + 1 < gridSizeX * 2 && rooms[x + 1, y] != null;
            }
        }
    }
}
