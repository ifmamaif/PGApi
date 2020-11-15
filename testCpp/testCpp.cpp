// testCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../PG_Library/MazeWorm.h"

#include <time.h>

int main()
{
    //char** da = GenerateMazeWorm(10, 10, 3);
    //ShowMazeWorm(da, 20, 20);

    //std::cout << PrintHello();
    //
    //std::cout << "Hello World!\n";

    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    //printf("Current local time and date: %s", asctime(timeinfo));

    std::cout << timeinfo->tm_mon << " "
        << timeinfo->tm_mday << " "
        << timeinfo->tm_hour << " "
        << timeinfo->tm_min << " "
        << timeinfo->tm_sec
        << std::endl;

    system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
