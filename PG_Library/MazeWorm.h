#pragma once

#include "PreDefines.h"

extern "C"
{
	EXPORT_API int** GenerateMazeWorm(int worldSizeX, int worldSizeY, int numberOfRooms);

	EXPORT_API int GetValueMazeWorm(int** buffer, int x, int y);

	EXPORT_API void DeleteMazeWorm(int** buffer, int x, int y);

	EXPORT_API void ShowMazeWorm(int** buffer,int worldSizeX, int worldSizeY);
}