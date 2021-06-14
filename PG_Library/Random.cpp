#include "pch.h"
#include "Random.h"

#include "Math.h"

#include <mutex>

static int InițializeazăSămânța();

static const unsigned int ms_Scale = 1103515245;
static const unsigned int ms_Offset = 12345;
static const unsigned int ms_Module = 2147483647; // 2 << 31; INT_MAX

static unsigned int ms_Seed = InițializeazăSămânța();
static unsigned int ms_Seed_s = InițializeazăSămânța();
static std::mutex gs_Mutex;

int AleatoriuIntreg()
{
    return ms_Seed = ((ms_Scale * ms_Seed) + ms_Offset) % ms_Module;
}

int Aleatoriu_Securizat()
{
    gs_Mutex.lock();
    const int newSeed = ms_Seed_s = ((ms_Scale * ms_Seed_s) + ms_Offset) % ms_Module;
    gs_Mutex.unlock();
    return newSeed;
}

int AleatoriuÎntre(int minInclusive, int maxExclusive)
{
    return minInclusive + (AleatoriuIntreg() % (maxExclusive - minInclusive + 1));
}

// Random value between 0 and 1
float Aleatoriu01f()
{
    return ((float)AleatoriuIntreg()) / (float)(ms_Module);
}

int AleatoriuNouăSămânța(int newseed)
{
    return ms_Seed = ((ms_Scale * newseed) + ms_Offset) % ms_Module;
}

const struct tm* ObțineOraCurentă()
{
	static time_t timpulBrut;
	time(&timpulBrut);
	return localtime(&timpulBrut);
}

static int InițializeazăSămânța()
{
    const struct tm* timeinfo = ObțineOraCurentă();
    unsigned int newSeed = timeinfo->tm_mon * 100000000
        + timeinfo->tm_mday * 1000000
        + timeinfo->tm_hour * 10000
        + timeinfo->tm_min * 100
        + timeinfo->tm_sec;

    return newSeed;
}