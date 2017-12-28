// Author: —k Žq‹B

#include "PerlinNoise.h"
#include <math.h>

float Noise(int x);
float SmoothedNoise(int x);
float InterpolatedNoise(float x);

float PerlinNoise(float x)
{

	float total = 0.0f;

	for (int i = 0; i < kOctaversNum; i++)
	{
		float frequency = powf(2.0f,(float)i);
		float amplitude = powf(kPersistence,(float)i);
		total = total + InterpolatedNoise(x * frequency) * amplitude;
	}

	return total;
}

float Noise(int x)
{
	x = (x << 13) ^ x;
	return (float)(1.0 - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float SmoothedNoise(int x)
{
	return Noise(x) / 2 + Noise(x - 1) / 4 + Noise(x + 1) / 4;
}

float InterpolatedNoise(float x)
{
	int int_x = (int)x;

	float v1 = SmoothedNoise(int_x);
	float v2 = SmoothedNoise(int_x + 1);

	return v1 + (v2 - v1)*(x - int_x);
}