#include "test.h"

void initTest(Object * thiz)
{
	SetThis(Test);
	thiz->polygon = newPolygon(thiz);

	float scale = 0.4f;
	thiz->transform->scale = Vector3(scale, scale, 0);
	thiz->transform->position = Vector3(Randomf(-2000, 2000), Randomf(-1000, 1000), 0);

	thizz->timer = newTimer(Randomf(0.2f, 0.7f));
	thizz->speed = Vector3(Randomf(-50, 50), Randomf(-50, 50), 0.0f)*2;
	thizz->targetZ = Randomf(1, 5);
	thizz->targetC = ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 50);
	Polygon_SetColor(thiz->polygon, thizz->targetC);
}

void updateTest(Object * thiz)
{
	SetThis(Test);
	if (Timer_TimeUp(thizz->timer))
	{
		thizz->speed = Vector3(Randomf(-50, 50), Randomf(-50, 50), 0.0f);
		Timer_Reset(thizz->timer);
	}
	thiz->transform->position += thizz->speed*GetDeltaTime();
	thiz->transform->position.z = Lerpf(thiz->transform->position.z, thizz->targetZ, 3 * GetDeltaTime());
	//Polygon_SetOpacity(thiz->polygon, (1-(thizz->targetZ - thiz->transform->position.z)/(10+ thizz->targetZ)));
}

void uninitTest(Object * thiz)
{
	SetThis(Test);
	SafeDelete(thizz->timer);
}
