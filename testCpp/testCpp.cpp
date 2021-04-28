﻿// testCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../PG_Library/MazeWorm.h"
#include "../PG_Library/Random.h"
#include "../PG_Library/KenPerlin.h"
#include "../PG_Library/ValueNoise.h"
#include "../PG_Library/Constants.h"
#include "../PG_Library/PerlinNoiseImproved.h"
#include "../PG_Library/SimplexNoise.h"
#include "../PG_Library/Noise.h"
#include "../PG_Library/Math.h"

#include <time.h>

void GetMinMaxPerlinNoise_Improved(float (*f)(float,float,float))
{
	//Minim - 0.878596,  -0.900608 ,  -0.837054 ,  -0.863498 ,  -0.837278
	//Maxim 0.90416 , 0.898667 ,  0.959816 , 0.958262,0.927884
	float min = 1;
	float max = -1;

	for (int i = 0; i < 10000; i++)
	{
		float x = Randf()*1000;
		float y = Randf()*1000;
		float z = Randf()*1000;

		float res = f(x, y, z);
		if (res < min)
		{
			min = res;
		}
		if (res > max)
		{
			max = res;
		}
	}

	float increment = 0;
	for (int i = 0; i < 10001; i++)
	{
		float res = f(increment, increment, increment);
		if (res < min)
		{
			min = res;
		}
		if (res > max)
		{
			max = res;
		}
		increment += 0.00001f;
	}

	std::cout << "Minim " << min << std::endl;
	std::cout << "Maxim " << max << std::endl;
}

int TestPerlinNDim()
{
    for (int i = 0; i < 100000; i++)
    {
        float x = Randf()*100;
        float y = Randf()*100;
        float z = Randf()*100;

        float result1, result2;
		float* results1, *results2;
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


void da()
{
	float min = 1;
	float max = -1;

	int width = 100;
	int height = 100;

	int scale = 20;
	int offset = 800;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float x = (float)i * scale + offset;// / width;
			float y = (float)j * scale + offset;// / height;

			auto noise = Fbm(x, y, 0, 8, 1, 0.5f, 0.0005f, 2);
			//100 -0.0371563
			//100 0.147357

			//200 -0.0728363    -400
			//200 0.206986		+600

			//400 -0.137426     -600
			//400 0.249434      +400

			//500 -0.201062
			//500 0.249434

			noise *= 2;
			noise += 1;
			noise /= 2;	
			noise = RoundToInt(noise);

			if (min > noise)
			{
				min = noise;
			}
			if (max < noise)
			{
				max = noise;
			}

			std::cout << noise<< " " ;
		}
		std::cout << std::endl;
	}

	std::cout << min << std::endl;
	std::cout << max << std::endl;
}

int main(int argc,char* argv[])
{
	//GetMinMaxPerlinNoise_Improved(ClassicPerlinNoise3D);
	//GetMinMaxPerlinNoise_Improved(PerlinNoise_Improved3D);
	//GetMinMaxPerlinNoise_Improved(SimplexNoise3D);
	//std::cout << TestPerlinNDim() << std::endl;
	//std::cout << TestValueNoise() << std::endl;
	da();
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
