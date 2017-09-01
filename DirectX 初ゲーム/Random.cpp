#include "Random.h"

inline int round(float x)
{
	return (int)(x + 0.5f);
}

void InitRandom(void)
{
	srand((unsigned)time(NULL));
}

int Random(int min, int max)
{
	int range = max - min;
	if (range <= 0)
		return min;
	else
		return round(min + ( rand() * (float)range / RAND_MAX));
}

float Randomf(float min, float max)
{
	float range = max - min;
	if (range < 1.0e-35f)
		return min;
	else
		return (min + (rand() * range / RAND_MAX));
}
