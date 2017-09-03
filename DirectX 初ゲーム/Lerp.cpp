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

Vector3 LerpV3(Vector3 a, Vector3 b, float t)
{
	if (t >= 1)
		return b;
	else if (t <= 0)
		return a;
	else
		return (a + (b - a)*t);
}

//Color LerpC(Color a, Color b, float t)
//{
//	if (t >= 1)
//		return b;
//	else if (t <= 0)
//		return a;
//	else
//	{
//		return (a + (b - a)*t);
//	}
//}
