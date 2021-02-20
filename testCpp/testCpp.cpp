// testCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../PG_Library/MazeWorm.h"
#include "../PG_Library/Random.h"
#include "../PG_Library/KenPerlin.h"

#include <time.h>

int TestPerlinNDim()
{
    for (int i = 0; i < 100000; i++)
    {
        double x = Randf()*100;
        double y = Randf()*100;
        double z = Randf()*100;

		double result1 = ClassicPerlinNoise3D(x, y, z);
		double result2 = PerlinNoiseND(3, x, y, z);
		if (result1 != result2)
		{
			return 0;
		}
    }
    return 1;
}



int main(int argc,char* argv[])
{
    //auto** da = GenerateMazeWorm(4, 4, 32);
    //ShowMazeWorm(da, 8, 8);

    std::cout << TestPerlinNDim();


    //system("pause");
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
