using System.Collections.Generic;
using UnityEngine;
using static MazeWhorm;

public class LevelGeneration : GenericBehaviour
{
    private GameObject m_Level = null;
    public GameObject roomWhiteObj = null;

    private readonly Vector2Int WORLD_SIZE = new Vector2Int(4, 4);
    private const int NUMBER_OF_ROOMS = 32;
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

        var m_Rooms = MazeWhorm.Generate(WORLD_SIZE, NUMBER_OF_ROOMS);
        DrawMap(m_Rooms); //instantiates objects to make up a map
    }

    private void DrawMap(Room[,] m_Rooms)
    {
        for (int i = 0; i < WORLD_SIZE.x * 2; i++)
            for (int j = 0; j < WORLD_SIZE.y * 2; j++)
            {
                Room room = m_Rooms[i, j];
                if (room == null)
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
                mapper.up = room.doorTop;
                mapper.down = room.doorBot;
                mapper.right = room.doorRight;
                mapper.left = room.doorLeft;
            }
    }
}