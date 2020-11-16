#pragma once

#include "PreDefines.h"

extern "C"
{
	// Generate a 2D map 
	// 0 - empty cell 
	// 1 - link with top
	// 2 - link with bot
	// 4 - link with left
	// 8 - link with right
	EXPORT_API int** GenerateMazeWorm(int worldSizeX, int worldSizeY, int numberOfRooms);

	EXPORT_API int GetValueMazeWorm(int** buffer, int x, int y);

	EXPORT_API void DeleteMazeWorm(int** buffer, int x, int y);

	EXPORT_API void ShowMazeWorm(int** buffer,int worldSizeX, int worldSizeY);
}