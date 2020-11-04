#include "pch.h"
#include "Random.h"

static unsigned int ms_Scale = 1103515245;
static unsigned int ms_Offset = 12345;
static unsigned int ms_Module = 2147483647; // 2 << 31;
static unsigned int ms_Seed = 691;

unsigned int Random::Rand()
{
    return ms_Seed = ((ms_Scale * ms_Seed) + ms_Offset) % ms_Module;
}

unsigned int Random::Rand(unsigned int a, unsigned int b)
{
    return ((Rand() % b) + a) % b;
}

unsigned int Random::Rand(unsigned int newseed)
{
    return ms_Seed = ((ms_Scale * newseed) + ms_Offset) % ms_Module;
}