// testCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../PG_Library/MazeWorm.h"
#include "../PG_Library/Random.h"
#include "../PG_Library/KenPerlin.h"
#include "../PG_Library/ValueNoise.h"
#include "../PG_Library/Constants.h"

#include <time.h>

int TestPerlinNDim()
{
    for (int i = 0; i < 100000; i++)
    {
        double x = Randf()*100;
        double y = Randf()*100;
        double z = Randf()*100;

        double result1, result2;
		double* results1, *results2;
		int* resultsI1, * resultsI2;

		results1 = ClassicPerlinNoise1D_Test_Input(x);
		results2 = PerlinNoiseND_Test_Input(1, x);
		if (results1[0] != results2[0])
		{
			return 0;
		}

		resultsI1 = ClassicPerlinNoise1D_Test_UnitGrid(x);
		resultsI2 = PerlinNoiseND_Test_UnitGrid(1, x);
		if (resultsI1[0] != resultsI2[0])
		{
			return 0;
		}

		results1 = ClassicPerlinNoise1D_Test_Relative(x);
		results2 = PerlinNoiseND_Test_Relative(1, x);
		if (results1[0] != results2[0])
		{
			return 0;
		}

		resultsI1 = ClassicPerlinNoise1D_Test_Wrap(x);
		resultsI2 = PerlinNoiseND_Test_Wrap(1, x);
		if (resultsI1[0] != resultsI2[0])
		{
			return 0;
		}

		resultsI1 = ClassicPerlinNoise1D_Test_Gradients(x);
		resultsI2 = PerlinNoiseNDArray_Test_Gradients(1, x);
		for (int i = 0; i < 2; i++)
		{
			if (resultsI1[i] != resultsI2[i])
			{
				return 0;
			}
		}

		result1 = ClassicPerlinNoise1D(x);
		result2 = PerlinNoiseND(1, x);
		if (result1 != result2)
		{
			return 0;
		}

		result1 = ClassicPerlinNoise2D(x, y);
		result2 = PerlinNoiseND(2, x, y);
		if (result1 != result2)
		{
			return 0;
		}

		result1 = ClassicPerlinNoise3D(x, y, z);
		result2 = PerlinNoiseND(3, x, y, z);
		if (result1 != result2)
		{
			return 0;
		}
    }
    return 1;
}

int TestValueNoise()
{
	const float input[][2] = { {0,0} };


	for (int i = 0; i < 1; i++)
	{
		float x = input[i][0];
		float y = input[i][1];
		float result1, result2;

		result1 = ValueNoise1D(x);
		result2 = ValueNoiseND(1, x);
		if (result1 != result2)
		{
			return 0;
		}

		result1 = ValueNoise2D(x, y);
		result2 = ValueNoiseND(2, x, y);
		if (result1 != result2)
		{
			return 0;
		}
	}

	return 1;
}


int main(int argc,char* argv[])
{

	std::cout << TestPerlinNDim() << std::endl;
	//std::cout << TestValueNoise() << std::endl;

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
