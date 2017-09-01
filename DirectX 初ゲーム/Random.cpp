#include "Random.h"

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
		return (int)(min + ( rand() / (float)RAND_MAX * (range+1.0f) ));
}

float Randomf(float min, float max)
{
	float range = max - min;
	if (range <= 0.0001f)
		return min;
	else
		return (min + (rand() / (float)RAND_MAX * (range + 1.0f)));

}
