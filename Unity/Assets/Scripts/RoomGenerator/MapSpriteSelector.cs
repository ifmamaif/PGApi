using UnityEngine;

public class MapSpriteSelector : MonoBehaviour
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