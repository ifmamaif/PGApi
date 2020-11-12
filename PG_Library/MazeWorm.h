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
	EXPORT_API char** GenerateMazeWorm(int worldSizeX, int worldSizeY, int numberOfRooms);

	EXPORT_API void ShowMazeWorm(char** buffer,int worldSizeX, int worldSizeY);

	EXPORT_API const char* DADA23();

	EXPORT_API const char* PrintHello();
}