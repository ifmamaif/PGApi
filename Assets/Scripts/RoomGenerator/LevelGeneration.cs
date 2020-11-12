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
    private static extern IntPtr DADA23();

    [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
    private static extern int DADA();

    [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr PrintHello();

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

        Debug.Log(Marshal.PtrToStringAnsi(PrintHello()));
        //Debug.Log((DADA().ToString()));

        char[,] m_Rooms = new char[WORLD_SIZE.x,WORLD_SIZE.y];
        //Marshal.PtrToStructure(GenerateMazeWorm(WORLD_SIZE.x, WORLD_SIZE.y,NUMBER_OF_ROOMS), m_Rooms);
        //m_Rooms = Marshal.PtrToStructure<char[,]>(GenerateMazeWorm(WORLD_SIZE.x, WORLD_SIZE.y, NUMBER_OF_ROOMS));
        IntPtr pUnmanagedArray = GenerateMazeWorm(WORLD_SIZE.x, WORLD_SIZE.y, NUMBER_OF_ROOMS);
        for(int i=0;i< WORLD_SIZE.x;i++)
        {
            IntPtr pDoubleArray = (IntPtr)((int)pUnmanagedArray + (Marshal.SizeOf(typeof(char)) * (WORLD_SIZE.y * i)));
            char[] tmp = new char[WORLD_SIZE.y];
            Marshal.Copy(pDoubleArray, tmp, 0, WORLD_SIZE.y);
        
            for(int j=0;j<WORLD_SIZE.y;j++)
            {
                m_Rooms[i,j] = tmp[j];
            }
        }
        
        //var m_Rooms = MazeWhorm.Generate(WORLD_SIZE, NUMBER_OF_ROOMS);
        DrawMap(m_Rooms); //instantiates objects to make up a map
    }

    private void DrawMap(char[,] m_Rooms)
    {
        for (int i = 0; i < WORLD_SIZE.x * 2; i++)
            for (int j = 0; j < WORLD_SIZE.y * 2; j++)
            {
                char room = m_Rooms[i, j];
                if (room == 0)
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
                mapper.up = ((room & 1) == 1); //room.doorTop;
                mapper.down = ((room & 2) == 2);
                mapper.left = ((room & 4) == 4);
                mapper.right = ((room & 8) == 8);
            }
    }
}