#include "test.h"

void initTest(Object * thiz)
{
	SetThis(Test);
	thiz->polygon = newPolygon(thiz);
	Polygon_SetColor(thiz->polygon, ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 200));

	float scale = Randomf(0.3f, 0.5f);
	thiz->transform->scale = Vector3(scale, scale, 0);
	thiz->transform->position = Vector3(Randomf(0, 1280), Randomf(0, 720), 0.0f);

	thisTest->timer = newTimer(Randomf(0.2f, 0.7f));
	thisTest->speed = Vector3(Randomf(-50, 50), Randomf(-50, 50), 0.0f)*2;
}

void updateTest(Object * thiz)
{
	SetThis(Test);
	if (Timer_TimeUp(thisTest->timer))
	{
		thisTest->speed = Vector3(Randomf(-50, 50), Randomf(-50, 50), 0.0f);
		Timer_Reset(thisTest->timer);
	}
	thiz->transform->position += thisTest->speed*GetDeltaTime();
}

void uninitTest(Object * thiz)
{
	SetThis(Test);
	SafeDelete(thisTest->timer);
}
