using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

public class LevelGeneration : GenericBehaviour
{
    private GameObject m_Level = null;
    public GameObject roomWhiteObj = null;

    private readonly Vector2Int WORLD_SIZE = new Vector2Int(4, 4);
    private const int NUMBER_OF_ROOMS = 32;
    private const string PATH_SPRITE = "Prefabs/MapSprite";

    [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr GenerateMazeWorm(int worldSizeX, int worldSizeY, int numberOfRooms);

    [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
    private static extern void DeleteMazeWorm(IntPtr buffer, int x, int y);

    [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
    private static extern int GetValueMazeWorm(IntPtr buffer, int x, int y);

    private const char NO_DOORS = (char)0;
    private const char DOOR_Possible = (char)1;
    private const char DOOR_BOTTOM = (char)2;
    private const char DOOR_LEFT = (char)4;
    private const char DOOR_TOP = (char)8;
    private const char DOOR_RIGHT = (char)16;

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

        IntPtr bufferMaze = GenerateMazeWorm(WORLD_SIZE.x, WORLD_SIZE.y, NUMBER_OF_ROOMS);
        DrawMap(bufferMaze); //instantiates objects to make up a map
        DeleteMazeWorm(bufferMaze, WORLD_SIZE.x * 2, WORLD_SIZE.y * 2);
    }

    private void DrawMap(IntPtr bufferMaze)
    {
        for (int i = 0; i < WORLD_SIZE.x * 2; i++)
            for (int j = 0; j < WORLD_SIZE.y * 2; j++)
            {
                var room = GetValueMazeWorm(bufferMaze,i, j);
                if (room == NO_DOORS)
                {
                    continue; //skip where there is no room
                }

                Vector2 drawPos = new Vector2(i,j) - WORLD_SIZE;
                drawPos.x *= 16;//aspect ratio of map sprite
                drawPos.y *= 8;

                //create map obj and assign its variables
                GameObject cell = Instantiate(roomWhiteObj, drawPos, Quaternion.identity);
                cell.transform.SetParent(m_Level.transform);
                MapSpriteSelector mapper = cell.GetComponent<MapSpriteSelector>();

                mapper.type = drawPos == Vector2Int.zero ? 1 : 0;

                mapper.up = ((room & DOOR_TOP) == DOOR_TOP);
                mapper.down = ((room & DOOR_BOTTOM) == DOOR_BOTTOM);
                mapper.left = ((room & DOOR_LEFT) == DOOR_LEFT);
                mapper.right = ((room & DOOR_RIGHT) == DOOR_RIGHT);
            }
    }
}