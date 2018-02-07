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
		Color result;
		result.r = (unsigned char)Lerpf(a.r, b.r, t);
		result.g = (unsigned char)Lerpf(a.g, b.g, t);
		result.b = (unsigned char)Lerpf(a.b, b.b, t);
		result.a = (unsigned char)Lerpf(a.a, b.a, t);
		//for (int i = 0; i < 4; i++)
		//{
		//	result += (int)Lerpf((float)((a & (0x000000ff << (8 * i))) >> (8 * i)), (float)((b & (0x000000ff << (8 * i))) >> (8 * i)), t) << (8 * i);
		//}
		return result;
	}
}
