#include "Test_PerlinNoise.h"

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

void Test_PerlinNoise()
{
	const auto iteration = 5000;
	float min = 1;
	float max = -1;

	auto testMinMax = [&](float value)
	{
		if (value < min)
		{
			min = value;
		}
		if (value > max)
		{
			max = value;
		}
	};

	for (int i = 0; i < iteration; i++)
	{
		auto x = Randf() * 1000;
		auto y = Randf() * 1000;
		auto z = Randf() * 1000;

		float res = PerlinNoise_Classic3D(x, y, z);
		testMinMax(res);
		auto cent = (float)i / iteration;
		res = PerlinNoise_Classic3D(cent, cent, cent);
		testMinMax(res);
		res = PerlinNoise_Classic3D(cent + x, cent + y, cent + z);
		testMinMax(res);
	}

	std::cout << "Min: " << min << std::endl;
	std::cout << "Max: " << max << std::endl;
}

void GetMinMaxNoise(float (*f)(float, float, float))
{
	float min = 1;
	float max = -1;
	auto iteration = 5000;
	float increment = 0;

	auto testMinMax = [&](float value)
	{
		if (value < min)
		{
			min = value;
		}
		if (value > max)
		{
			max = value;
		}
	};

	for (int i = 0; i < iteration; i++)
	{
		float x = Randf() * 1000;
		float y = Randf() * 1000;
		float z = Randf() * 1000;

		float res = f(x, y, z);
		testMinMax(res);
		res = f(increment, increment, increment);
		testMinMax(res);
		auto cent = (float)i / iteration;
		res = f(cent, cent, cent);
		testMinMax(res);
		res = f(cent + x, cent + y, cent + z);
		testMinMax(res);
		increment += 0.00001f;
	}


	std::cout << "Minim " << min << std::endl;
	std::cout << "Maxim " << max << std::endl;
}

void GetMinMaxNoise2D(float (*f)(float, float))
{
	float min = 1;
	float max = -1;
	auto iteration = 5000;
	float increment = 0;

	auto testMinMax = [&](float value)
	{
		if (value < min)
		{
			min = value;
		}
		if (value > max)
		{
			max = value;
		}
	};

	for (int i = 0; i < iteration; i++)
	{
		float x = Randf() * 1000;
		float y = Randf() * 1000;

		float res = f(x, y);
		testMinMax(res);
		res = f(increment, increment);
		testMinMax(res);
		auto cent = (float)i / iteration;
		res = f(cent, cent);
		testMinMax(res);
		res = f(cent + x, cent + y);
		testMinMax(res);
		increment += 0.00001f;
	}


	std::cout << "Minim " << min << std::endl;
	std::cout << "Maxim " << max << std::endl;
}

void TestVersions(float (*f1)(float, float), float (*f2)(float, float))
{
	float min = 1;
	float max = -1;
	auto iteration = 5000;
	float increment = 0;
	
	for (int i = 0; i < iteration; i++)
	{
		float x = Randf() * 1000;
		float y = Randf() * 1000;

		float res1 = f1(x, y);
		float res2 = f2(x, y);
		if (res1 != res2)
		{
			return;
		}

		res1 = f1(increment, increment);
		res2 = f2(increment, increment);
		if (res1 != res2)
		{
			return;
		}

		auto value = (float)i / iteration;
		res1 = f1(value, value);
		res2 = f2(value, value);
		if (res1 != res2)
		{
			return;
		}

		res1 = f1(value + x, value + y);
		res2 = f1(value + x, value + y);
		if (res1 != res2)
		{
			return;
		}
		increment += 0.00001f;
	}


	std::cout << "Minim " << min << std::endl;
	std::cout << "Maxim " << max << std::endl;
}
