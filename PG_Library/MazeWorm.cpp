#include "pch.h"
#include "MazeWorm.h"

#include "Math.h"
#include "Random.h"
#include "Utils.h"
#include "Queue.h"
#include "Vector2Int.h"

const char NO_DOORS = (char)0;
const char DOOR_POSSIBLE = (char)1;
const char DOOR_BOTTOM = (char)2;
const char DOOR_LEFT = (char)4;
const char DOOR_TOP = (char)8;
const char DOOR_RIGHT = (char)16;

const int MAX_ITERATIONS = 100;
const int HALF_ITERATIONS = MAX_ITERATIONS / 2;

int CheckInputData(int worldSizeX, int worldSizeY, int numberOfRooms, Vector2Int* vectorOut)
{
    int maximFit = worldSizeX * 2 * worldSizeY * 2;
    if (numberOfRooms >= maximFit)
    {
        numberOfRooms = maximFit;
    }

    vectorOut->x = worldSizeX;
    vectorOut->y = worldSizeY;

    return numberOfRooms;
}

int NumberOfNeighbors(const Vector2Int& checkingPos, const Queue<Vector2Int>& usedPositions)
{
    int ret = 0;
    ret += usedPositions.Contains(checkingPos + Vector2Int::right) ? 1 : 0;
    ret += usedPositions.Contains(checkingPos + Vector2Int::left) ? 1 : 0;
    ret += usedPositions.Contains(checkingPos + Vector2Int::up) ? 1 : 0;
    ret += usedPositions.Contains(checkingPos + Vector2Int::down) ? 1 : 0;

    return ret;
}

void GetNewPosition(int index, Vector2Int* vectorOut,const Queue<Vector2Int>& g_TakenPositions)
{
    const Vector2Int& value = g_TakenPositions[index];

	int upDown = Aleatoriu01f() < 0.5f ? 1 : 0;
	int positive = Aleatoriu01f() < 0.5f ? 1 : -1;

    vectorOut->x = value.x + (1 - upDown) * positive;
    vectorOut->y = value.y + (upDown * positive);
}

void NewPosition(Vector2Int* checkingPos,const Vector2Int& g_GridSize, const Queue<Vector2Int>& g_TakenPositions)
{
    int index;
    do
    {
        index = RotunjireLaÎntregf(Aleatoriu01f() * (g_TakenPositions.Size() - 1));
        GetNewPosition(index, checkingPos,g_TakenPositions);
    } while (g_TakenPositions.Contains(*checkingPos) ||
             checkingPos->OutSide(g_GridSize));
}

void SelectiveNewPosition(Vector2Int* checkingPos, const Vector2Int& g_GridSize, const Queue<Vector2Int>& g_TakenPositions)
{
    int inc;
    do
    {
        inc = 0;
        int index;
        do
        {
            index = RotunjireLaÎntregf(Aleatoriu01f() * (g_TakenPositions.Size() - 1));
            inc++;
        } while (NumberOfNeighbors(g_TakenPositions[index], g_TakenPositions) > 1 && inc < MAX_ITERATIONS);

        GetNewPosition(index, checkingPos,g_TakenPositions);

    } while (g_TakenPositions.Contains(*checkingPos)||
             checkingPos->OutSide(g_GridSize));

}

int** CreateRooms(int numberOfRooms,const Vector2Int& g_GridSize, Queue<Vector2Int>* g_TakenPositions)
{
    int rows = g_GridSize.x * 2;
    int columns = g_GridSize.y* 2;

    int** rooms = new int*[rows];
    for (int i = 0; i < rows; i++)
    {
        rooms[i] = new int[columns];
        for (int j = 0; j < columns; j++)
        {
            rooms[i][j] = 0;
        }
    }

    rooms[g_GridSize.x][g_GridSize.y] = DOOR_POSSIBLE;
    g_TakenPositions->PushBack(Vector2Int::zero);

    const float RANDOM_COMPARE_START = 0.2f;
    const float RANDOM_COMPARE_END = 0.01f;
   
    for (int i = 0; i < numberOfRooms - 1; i++)
    {
        float randomPerc = i / ((float)numberOfRooms - 1);
        float randomCompare = InterpolareLiniarăf(RANDOM_COMPARE_START, RANDOM_COMPARE_END, randomPerc);

        Vector2Int newPosition;
        NewPosition(&newPosition,g_GridSize,*g_TakenPositions);

        if (NumberOfNeighbors(newPosition, *g_TakenPositions) > 1 && Aleatoriu01f() > randomCompare)
        {
            int iterations = 0;
            do
            {
                SelectiveNewPosition(&newPosition,g_GridSize,*g_TakenPositions);
                iterations++;
            } while (NumberOfNeighbors(newPosition, *g_TakenPositions) > 1 && iterations < MAX_ITERATIONS);
        }
        rooms[newPosition.x + g_GridSize.x][newPosition.y + g_GridSize.y] = DOOR_POSSIBLE;
        g_TakenPositions->PushFront(newPosition);
    }

    return rooms;
}

void SetRoomDoors(int** rooms,const Vector2Int& g_GridSize)
{
    for (int x = 0; x < g_GridSize.x * 2; x++)
    {
        for (int y = 0; y < g_GridSize.y * 2; y++)
        {
            if (rooms[x][y] == NO_DOORS)
            {
                continue;
            }

            rooms[x][y] += (y - 1 >= NO_DOORS && rooms[x][y - 1] != NO_DOORS) ? DOOR_BOTTOM : NO_DOORS;
            rooms[x][y] += (y + 1 < g_GridSize.y * 2 && rooms[x][y + 1] != NO_DOORS) ? DOOR_TOP : NO_DOORS;
            rooms[x][y] += (x - 1 >= NO_DOORS && rooms[x - 1][y] != NO_DOORS) ? DOOR_LEFT : NO_DOORS;
            rooms[x][y] += (x + 1 < g_GridSize.x * 2 && rooms[x + 1][y] != NO_DOORS) ? DOOR_RIGHT : NO_DOORS;
        }
    }
}

int** GenerateMazeWorm(int worldSizeX, int worldSizeY, int numberOfRooms)
{
    Vector2Int g_GridSize;
    Queue<Vector2Int> g_TakenPositions;

    numberOfRooms = CheckInputData(worldSizeX, worldSizeY, numberOfRooms,&g_GridSize);

    auto result = CreateRooms(numberOfRooms,g_GridSize,&g_TakenPositions); 
    SetRoomDoors(result, g_GridSize);                                      
    return result;
}

int GetValueMazeWorm(int** buffer, int x, int y)
{
    return buffer[x][y];
}

void DeleteMazeWorm(int** buffer, int x, int y)
{
    for (int i = 0; i < x * 2; i++)
    {
        delete(buffer[i]);
    }
    delete(buffer);
}

void ShowMazeWorm(int** buffer, int worldSizeX, int worldSizeY)
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