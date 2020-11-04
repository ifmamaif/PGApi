#pragma once

// Generate a 2D map 
// 0 - empty cell 
// 1 - link with top
// 2 - link with bot
// 4 - link with left
// 8 - link with right
char** Generate(int worldSizeX, int worldSizeY, int numberOfRooms);