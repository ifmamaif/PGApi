// testCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../PG_Library/MazeWorm.h"
#include "../PG_Library/Random.h"
#include "../PG_Library/KenPerlin.h"
#include "../PG_Library/ValueNoise.h"
#include "../PG_Library/Constants.h"
#include "../PG_Library/PerlinNoiseImproved.h"
#include "../PG_Library/SimplexNoise.h"
#include "../PG_Library/Noise.h"
#include "../PG_Library/Math.h"
#include "../PG_Library/PerlinUtils.h"

#include <ctime>
#include <cstdlib>
#include <iostream>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "Test_PerlinNoise.h"

void PRNG_C() {
	srand(time(NULL)); /* seed the generator */
	int rand1 = rand(); /* a pseudorandom integer between 0 and RAND_MAX */
	printf("Random number between 0 and %d: %d\n", RAND_MAX, (int)rand1);
	int min = 0;
	int max = 100;
	float rand2 = (float)rand() * max / RAND_MAX + 1;
	int round = (int)rand2;
	printf("Random number between %d and %d: %d (%f)\n", min, max, round, rand2);
	return;
}

int PRNG_Cplusplus() {
	srand(time(NULL));
	std::cout << "Random number between 0 and " << RAND_MAX << ": " << rand() << "\n"
		<< "Random number between 1 and 100: " << (rand() % 100) + 1 << std::endl;
	return 0;
}

void GradientsValue1()
{
	for (int i = 0; i < 16; i++)
	{
		std::cout << i << " " << Gradient1D(i, 1)<< std::endl;
	}
}

void GradientsValue2()
{
	for (int i = 0; i < 65; i++)
	{
		std::cout << i << ": "<< " 2,3 " << Gradient2D(i, 2,3) << std::endl;
	}
}

void TestMinMax()
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
			float x = (float)i * scale + offset;
			float y = (float)j * scale + offset;

			auto noise = Fbm(x, y, 0, 8, 1, 0.5f, 0.0005f, 2);

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
	TestVersions(SimplexNoise2D,noise_simplex_optimized);
}