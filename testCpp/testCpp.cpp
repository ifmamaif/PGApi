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
	srand(time(NULL)); /* inițializarea generatorului cu data prezentă */
	int valoareAleatoare1 = rand(); /* o valoare pseudo-aleatoare întreagă între 0 și RAND_MAX */
	printf("Un număr aleator între 0 și %d: %d\n", RAND_MAX, (int)valoareAleatoare1);
	int minim = 0;
	int maxim = 100;
	float valoareAleatoare2 = (float)rand() * maxim / RAND_MAX + 1;
	int rotunjire = (int)valoareAleatoare2;
	printf("Un număr aleator între %d și %d: %d (%f)\n", minim, maxim, rotunjire, valoareAleatoare2);
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

			auto noise = MișcareFracționatăBrowniană(x, y, 0, 8, 1, 0.5f, 0.0005f, 2);

			noise *= 2;
			noise += 1;
			noise /= 2;	
			noise = RotunjireLaÎntregf(noise);

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
	std::cout << "Hello";
}