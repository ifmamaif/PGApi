using System.Collections.Generic;
using UnityEngine;

public class LevelGeneration : GenericBehaviour
{
    private GameObject m_Level = null;
    private Vector2Int m_WorldSize = new Vector2Int(4, 4);
    private Room[,] m_Rooms = null;
    private List<Vector2Int> m_TakenPositions = null;
    private int m_NumberOfRooms = 32;
    private Vector2Int m_GridSize;

    public GameObject roomWhiteObj = null;

    private const string PATH_SPRITE = "Prefabs/MapSprite";

    public override void Start()
    {
        roomWhiteObj = (GameObject)Resources.Load(PATH_SPRITE, typeof(GameObject));
        Generate();
    }

    public override void Generate()
    {
        Destroy(m_Level);
        m_Level = new GameObject("Level");
        m_Level.transform.SetParent(gameObject.transform);

        if (roomWhiteObj == null)
        {
            roomWhiteObj = (UnityEngine.GameObject)Resources.Load(PATH_SPRITE, typeof(GameObject));
        }

        int maximFit = m_WorldSize.x * 2 * m_WorldSize.y * 2;
        if (m_NumberOfRooms >= maximFit)
        { // make sure we dont try to make more rooms than can fit in our grid
            m_NumberOfRooms = Mathf.RoundToInt(maximFit);
        }
        m_GridSize.x = Mathf.RoundToInt(m_WorldSize.x); //note: these are half-extents
        m_GridSize.y = Mathf.RoundToInt(m_WorldSize.y);
        CreateRooms(); //lays out the actual map
        SetRoomDoors(); //assigns the doors where rooms would connect
        DrawMap(); //instantiates objects to make up a map
    }

    private void CreateRooms()
    {
        //setup
        m_Rooms = new Room[m_GridSize.x * 2, m_GridSize.y * 2];
        m_Rooms[m_GridSize.x, m_GridSize.y] = new Room(Vector2.zero, 1);
        m_TakenPositions = new List<Vector2Int>() { Vector2Int.zero };

        //magic numbers
        const float RANDOM_COMPARE_START = 0.2f;
        const float RANDOM_COMPARE_END = 0.01f;
        //add rooms
        for (int i = 0; i < m_NumberOfRooms - 1; i++)
        {
            float randomPerc = i / ((float)m_NumberOfRooms - 1);
            float randomCompare = Mathf.Lerp(RANDOM_COMPARE_START, RANDOM_COMPARE_END, randomPerc);
            //grab new position
            Vector2Int checkPos = NewPosition();
            //test new position
            if (NumberOfNeighbors(checkPos, m_TakenPositions) > 1 && Random.value > randomCompare)
            {
                int iterations = 0;
                do
                {
                    checkPos = SelectiveNewPosition();
                    iterations++;
                } while (NumberOfNeighbors(checkPos, m_TakenPositions) > 1 && iterations < 100);
                if (iterations >= 50)
                {
                    // print("error: could not create with fewer neighbors than : " + NumberOfNeighbors(checkPos, takenPositions));
                }
            }
            //finalize position
            m_Rooms[(int)checkPos.x + m_GridSize.x, (int)checkPos.y + m_GridSize.y] = new Room(checkPos, 0);
            m_TakenPositions.Insert(0, checkPos);
        }
    }

    private Vector2Int NewPosition()
    {
        Vector2Int checkingPos;
        int index;
        do
        {
            index = Mathf.RoundToInt(Random.value * (m_TakenPositions.Count - 1)); // pick a random room

            checkingPos = GetNewPosition(index);

        } while (m_TakenPositions.Contains(checkingPos) || checkingPos.OutSide(m_GridSize)); //make sure the position is valid

        return checkingPos;
    }

    // method differs from the above in the two commented ways
    private Vector2Int SelectiveNewPosition()
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
                index = Mathf.RoundToInt(Random.value * (m_TakenPositions.Count - 1));
                inc++;
            } while (NumberOfNeighbors(m_TakenPositions[index], m_TakenPositions) > 1 && inc < 100);

            checkingPos = GetNewPosition(index);

        } while (m_TakenPositions.Contains(checkingPos) || checkingPos.OutSide(m_GridSize));

        if (inc >= 100)
        { // break loop if it takes too long: this loop isnt garuanteed to find solution, which is fine for this
            print("Error: could not find position with only one neighbor");
        }

        return checkingPos;
    }

    private Vector2Int GetNewPosition(int index)
    {
        //capture its x, y position
        int positionX = m_TakenPositions[index].x;
        int positionY = m_TakenPositions[index].y;

        bool UpDown = Random.value < 0.5f;              //randomly pick wether to look on hor or vert axis
        int positive = Random.value < 0.5f ? 1 : -1;    //pick whether to be positive or negative on that axis

        //find the position bnased on the above bools
        if (UpDown)
        {
            positionY += positive;
        }
        else
        {
            positionX += positive;
        }

        return new Vector2Int(positionX, positionY);
    }

    private int NumberOfNeighbors(Vector2Int checkingPos, List<Vector2Int> usedPositions)
    {
        int ret = 0; // start at zero, add 1 for each side there is already a room
        ret += usedPositions.Contains(checkingPos + Vector2Int.right) ? 1 : 0;
        ret += usedPositions.Contains(checkingPos + Vector2Int.left) ? 1 : 0;
        ret += usedPositions.Contains(checkingPos + Vector2Int.up) ? 1 : 0;
        ret += usedPositions.Contains(checkingPos + Vector2Int.down) ? 1 : 0;

        return ret;
    }

    private void DrawMap()
    {
        foreach (Room room in m_Rooms)
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
            cell.transform.SetParent(m_Level.transform);
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
        for (int x = 0; x < m_GridSize.x * 2; x++)
        {
            for (int y = 0; y < m_GridSize.y * 2; y++)
            {
                if (m_Rooms[x, y] == null)
                {
                    continue;
                }

                m_Rooms[x, y].doorBot = y - 1 >= 0 && m_Rooms[x, y - 1] != null;
                m_Rooms[x, y].doorTop = y + 1 < m_GridSize.y * 2 && m_Rooms[x, y + 1] != null;
                m_Rooms[x, y].doorLeft = x - 1 >= 0 && m_Rooms[x - 1, y] != null;
                m_Rooms[x, y].doorRight = x + 1 < m_GridSize.x * 2 && m_Rooms[x + 1, y] != null;
            }
        }
    }
}