#include "pch.h"
#include "Random.h"

#include "Math.h"

static int InitializeSeed();

static int ms_Scale = 1103515245;
static int ms_Offset = 12345;
static int ms_Module = 2147483647; // 2 << 31; INT_MAX
static int ms_Seed = InitializeSeed();//691;

int Rand()
{
    return ms_Seed = ((ms_Scale * ms_Seed) + ms_Offset) % ms_Module;
}

int RandBetween(int minInclusive, int maxExclusive)
{
    //return ((Rand() % b) + a) % b;
    return minInclusive + (Rand() % (maxExclusive - minInclusive + 1));
}

// Random value between 0 and 1
float Randf()
{
    return ((float)Rand()) / (float)(ms_Module);
}

int RandNewSeed(int newseed)
{
    return ms_Seed = ((ms_Scale * newseed) + ms_Offset) % ms_Module;
}

const struct tm* GetCurrentLocalTime()
{
    static time_t rawtime;
    struct tm* timeinfo = nullptr;

    time(&rawtime);
    return localtime(&rawtime);
}

static int InitializeSeed()
{
    const struct tm* timeinfo = GetCurrentLocalTime();
    unsigned int newSeed = timeinfo->tm_mon * 100000000
        + timeinfo->tm_mday * 1000000
        + timeinfo->tm_hour * 10000
        + timeinfo->tm_min * 100
        + timeinfo->tm_sec;

    return newSeed;
}