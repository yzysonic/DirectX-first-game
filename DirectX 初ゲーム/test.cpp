#include "test.h"


void initTest(Object * thiz)
{
	thiz->polygon = newPolygon(thiz);
	thiz->transform->position = Vector3(Randomf(0, 1280), Randomf(0, 720), 0.0f);
}

void updateTest(Object * thiz)
{
	static float timer = 0;
	timer += GetDeltaTime();
	if (timer > 2)
	{
		//deleteObject(thiz);
		timer = 0;
	}
}

void uninitTest(Object * thiz)
{
}
