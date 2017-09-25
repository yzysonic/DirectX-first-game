#include "Lerp.h"

float Lerpf(float a, float b, float t)
{
	if (t >= 1)
		return b;
	else if (t <= 0)
		return a;
	else
		return (a + (b - a)*t);
}

Color LerpC(Color a, Color b, float t)
{
	if (t >= 1)
		return b;
	else if (t <= 0)
		return a;
	else
	{
		Color result = 0;
		for (int i = 0; i < 4; i++)
		{
			result += (int)Lerpf((float)((a & (0x000000ff << (8 * i))) >> (8 * i)), (float)((b & (0x000000ff << (8 * i))) >> (8 * i)), t) << (8 * i);
		}
		return result;
	}
}
