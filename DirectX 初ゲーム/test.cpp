#include "test.h"

Timer *timer = newTimer(1);

void initTest(Object * thiz)
{
	thiz->polygon = newPolygon(thiz);
	thiz->transform->position = Vector3(Randomf(0, 1280), Randomf(0, 720), 0.0f);
}

void updateTest(Object * thiz)
{
	static Vector3 speed;
	if (Timer_TimeUp(timer))
	{
		//deleteObject(thiz);
		speed = Vector3(Randomf(-100, 100), Randomf(-100, 100), 0.0f);
		Timer_Reset(timer);
	}
	thiz->transform->position += speed*GetDeltaTime();
}

void uninitTest(Object * thiz)
{
}
