using UnityEngine;
using System;

public class LevelGeneration : GenericBehaviour
{
    private GameObject m_Level = null;
    public GameObject roomWhiteObj = null;

    private readonly Vector2Int WORLD_SIZE = new Vector2Int(4, 4);
    private const int NUMBER_OF_ROOMS = 32;
    private const string PATH_SPRITE = "Prefabs/MapSprite";

    private const char NO_DOORS = (char)0;
    private const char DOOR_Possible = (char)1;
    private const char DOOR_BOTTOM = (char)2;
    private const char DOOR_LEFT = (char)4;
    private const char DOOR_TOP = (char)8;
    private const char DOOR_RIGHT = (char)16;

    public override void Constructor()
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

        //IntPtr bufferMaze = PGApi.PGApi.GenerateMazeWorm(WORLD_SIZE.x, WORLD_SIZE.y, NUMBER_OF_ROOMS);
        //DrawMap(bufferMaze); //instantiates objects to make up a map
        //DeleteMazeWorm(bufferMaze, WORLD_SIZE.x * 2, WORLD_SIZE.y * 2);
    }

    private void DrawMap(IntPtr bufferMaze)
    {
        for (int i = 0; i < WORLD_SIZE.x * 2; i++)
            for (int j = 0; j < WORLD_SIZE.y * 2; j++)
            {
                var room = PGApi.PGApi.GetValueMazeWorm(bufferMaze,i, j);
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

    private class MapSpriteSelector : MonoBehaviour
    {
        public Sprite spU, spD, spR, spL,
                spUD, spRL, spUR, spUL, spDR, spDL,
                spULD, spRUL, spDRU, spLDR, spUDRL;
        public bool up, down, left, right;
        public int type; // 0: normal, 1: enter
        public Color normalColor, enterColor;
        private Color mainColor;
        private SpriteRenderer rend;

        private void Start()
        {
            rend = GetComponent<SpriteRenderer>();
            mainColor = normalColor;
            PickSprite();
            PickColor();
        }

        private void PickSprite()
        { //picks correct sprite based on the four door bools
            if (up)
            {
                rend.sprite = down ? right ? left ? spUDRL : spDRU : left ? spULD : spUD : right ? left ? spRUL : spUR : left ? spUL : spU;
                return;
            }
            if (down)
            {
                rend.sprite = right ? left ? spLDR : spDR : left ? spDL : spD;
                return;
            }
            rend.sprite = right ? left ? spRL : spR : spL;
        }

        private void PickColor()
        { //changes color based on what type the room is
            if (type == 0)
            {
                mainColor = normalColor;
            }
            else if (type == 1)
            {
                mainColor = enterColor;
            }
            rend.color = mainColor;
        }
    }
}