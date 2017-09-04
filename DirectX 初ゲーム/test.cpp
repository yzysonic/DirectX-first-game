#include "test.h"

void initTest(Object * thiz)
{
	SetThis(Test);
	thiz->polygon = newPolygon(thiz);

	float scale = 0.4f;
	thiz->transform->scale = Vector3(scale, scale, 0);
	thiz->transform->position = Vector3(Randomf(-2000, 2000), Randomf(-1000, 1000), 0);

	thisTest->timer = newTimer(Randomf(0.2f, 0.7f));
	thisTest->speed = Vector3(Randomf(-50, 50), Randomf(-50, 50), 0.0f)*2;
	thisTest->targetZ = Randomf(1, 5);
	thisTest->targetC = ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 50);
	Polygon_SetColor(thiz->polygon, thisTest->targetC);
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
	thiz->transform->position.z = Lerpf(thiz->transform->position.z, thisTest->targetZ, 3 * GetDeltaTime());
	//Polygon_SetOpacity(thiz->polygon, (1-(thisTest->targetZ - thiz->transform->position.z)/(10+ thisTest->targetZ)));
}

void uninitTest(Object * thiz)
{
	SetThis(Test);
	SafeDelete(thisTest->timer);
}
