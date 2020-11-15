#include "pch.h"
#include "MazeWorm.h"

#include "Math.h"
#include "Random.h"
#include "Utils.h"
#include "Queue.h"

const char DOOR_TOP = 1;
const char DOOR_BOTTOM = 2;
const char DOOR_LEFT = 4;
const char DOOR_RIGHT = 8;

const int MAX_ITERATIONS = 100;
const int HALF_ITERATIONS = MAX_ITERATIONS / 2;

int g_GridSizeX, g_GridSizeY;

Queue<int> g_TakenPositionsX, g_TakenPositionsY;


int CheckInputData(int worldSizeX, int worldSizeY, int numberOfRooms)
{
    int maximFit = worldSizeX * 2 * worldSizeY * 2;
    if (numberOfRooms >= maximFit)
    { // make sure we dont try to make more rooms than can fit in our grid
        numberOfRooms = maximFit;
    }

    //note: these are half-extents
    g_GridSizeX = worldSizeX;
    g_GridSizeY = worldSizeY;

    return numberOfRooms;
}

int NumberOfNeighbors(int checkingPosX, int checkingPosY, Queue<int> usedPositionsX, Queue<int> usedPositionsY)
{
    int ret = 0; // start at zero, add 1 for each side there is already a room
    ret += (usedPositionsX.Contains(checkingPosX + 1) && usedPositionsY.Contains(checkingPosY + 0)) ? 1 : 0;
    ret += (usedPositionsX.Contains(checkingPosX - 1) && usedPositionsY.Contains(checkingPosY + 0)) ? 1 : 0;
    ret += (usedPositionsX.Contains(checkingPosX + 0) && usedPositionsY.Contains(checkingPosY + 1)) ? 1 : 0;
    ret += (usedPositionsX.Contains(checkingPosX + 0) && usedPositionsY.Contains(checkingPosY - 1)) ? 1 : 0;

    return ret;
}

void GetNewPosition(int index, int* xOut, int* yOut)
{
    //capture its x, y position
    int x = g_TakenPositionsX[index];
    int y = g_TakenPositionsY[index];

    int upDown = Randf() < 0.5f ? 1 : 0;        //randomly pick wether to look on hor or vert axis
    int positive = Randf() < 0.5f ? 1 : -1;    //pick whether to be positive or negative on that axis

    //find the position bnased on the above bools
    *xOut = x + (1 - upDown) * positive;
    *yOut = y + (upDown * positive);
}

void NewPosition(int* checkingPosX, int* checkingPosY)
{
    int index;
    do
    {
        index = RoundToInt(Randf() * (g_TakenPositionsX.Size() - 1)); // pick a random room
        GetNewPosition(index, checkingPosX, checkingPosY);
    } while ((g_TakenPositionsX.Contains(*checkingPosX) &&
        g_TakenPositionsY.Contains(*checkingPosY)) ||
        OutSidei(*checkingPosX, *checkingPosY, g_GridSizeX, g_GridSizeY)); //make sure the position is valid
}

// method differs from the above in the two commented ways
void SelectiveNewPosition(int* checkingPosX, int* checkingPosY)
{
    int inc;
    do
    {
        inc = 0;
        int index;
        do
        {
            //instead of getting a room to find an adject empty space, we start with one that only 
            //as one neighbor. This will make it more likely that it returns a room that branches out
            index = RoundToInt(Randf() * (g_TakenPositionsX.Size() - 1));
            inc++;
        } while (NumberOfNeighbors(g_TakenPositionsX[index], g_TakenPositionsY[index], g_TakenPositionsX, g_TakenPositionsY) > 1 && inc < MAX_ITERATIONS);

        GetNewPosition(index, checkingPosX, checkingPosY);

    } while ((g_TakenPositionsX.Contains(*checkingPosX) &&
        g_TakenPositionsY.Contains(*checkingPosY)) ||
        OutSidei(*checkingPosX, *checkingPosY, g_GridSizeX, g_GridSizeY));


    if (inc >= MAX_ITERATIONS)
    { // break loop if it takes too long: this loop isnt garuanteed to find solution, which is fine for this
      // print("Error: could not find position with only one neighbor");
    }
}

char** CreateRooms(int numberOfRooms)
{
    //setup
    char** rooms = (char**)malloc(sizeof(char*) * g_GridSizeX * 2);
    for (int i = 0; i < g_GridSizeX * 2; i++)
    {
        rooms[i] = (char*)malloc(sizeof(char) * g_GridSizeY * 2);
        for (int j = 0; j < g_GridSizeY * 2; j++)
        {
            rooms[i][j] = 0;
        }
    }
    
    //char** rooms = new char* [(g_GridSizeX * 2)];
    //for (int i = 0; i < g_GridSizeX * 2; i++)
    //{
    //    rooms[i] = new char [(g_GridSizeY * 2)];
    //    for (int j = 0; j < g_GridSizeY*2; j++)
    //        rooms[i][j] = 0;
    //}

    g_TakenPositionsX.PushBack(0);
    g_TakenPositionsY.PushBack(0);

    //magic numbers
    const float RANDOM_COMPARE_START = 0.2f;
    const float RANDOM_COMPARE_END = 0.01f;
    //add rooms
    for (int i = 0; i < numberOfRooms - 1; i++)
    {
        float randomPerc = i / ((float)numberOfRooms - 1);
        float randomCompare = Lerpf(RANDOM_COMPARE_START, RANDOM_COMPARE_END, randomPerc);
        //grab new position
        int x, y;
        NewPosition(&x, &y);
        //test new position
        if (NumberOfNeighbors(x, y, g_TakenPositionsX, g_TakenPositionsY) > 1 && Randf() > randomCompare)
        {
            int iterations = 0;
            do
            {
                SelectiveNewPosition(&x, &y);
                iterations++;
            } while (NumberOfNeighbors(x, y, g_TakenPositionsX, g_TakenPositionsY) > 1 && iterations < MAX_ITERATIONS);
            if (iterations >= HALF_ITERATIONS)
            {
                // print("error: could not create with fewer neighbors than : " + NumberOfNeighbors(checkPos, takenPositions));
            }
        }
        //finalize position
        g_TakenPositionsX.PushFront(x);
        g_TakenPositionsY.PushFront(y);
    }

    return rooms;
}

void SetRoomDoors(char** rooms)
{
    for (int x = 0; x < g_GridSizeX * 2; x++)
    {
        for (int y = 0; y < g_GridSizeY * 2; y++)
        {
            char& room = rooms[x][y];
            if (room == 0)
            {
                continue;
            }

            room = 0;
            room += (y - 1 >= 0 && rooms[x, y - 1] != 0) ? DOOR_BOTTOM : 0;
            room += (y + 1 < g_GridSizeY * 2 && rooms[x, y + 1] != 0) ? DOOR_TOP : 0;
            room += (x - 1 >= 0 && rooms[x - 1, y] != 0) ? DOOR_LEFT : 0;
            room += (x + 1 < g_GridSizeX * 2 && rooms[x + 1, y] != 0) ? DOOR_RIGHT : 0;
        }
    }
}

char** GenerateMazeWorm(int worldSizeX, int worldSizeY, int numberOfRooms)
{
    numberOfRooms = CheckInputData(worldSizeX, worldSizeY, numberOfRooms);

    auto result = CreateRooms(numberOfRooms);   //lays out the actual map
    SetRoomDoors(result);                       //assigns the doors where rooms would connect
    return result;
}

void ShowMazeWorm(char** buffer, int worldSizeX, int worldSizeY)
{
    for (int i = 0; i < worldSizeX; i++)
    {
        for (int j = 0; j < worldSizeY; j++)
        {
            std::cout << (int)(buffer[i][j]);
        }
        std::cout << std::endl;
    }
}

const char* DADA23()
{
    return "dada";
}

const char* PrintHello()
{
    return "Hello";
}
