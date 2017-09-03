#pragma once
#include "Core.h"

typedef struct _Test
{
	Object *base;
	Timer *timer;
	Vector3 speed;
	float targetZ;
	Color targetC;
}Test;

void initTest(Object *thiz);
void updateTest(Object *thiz);
void uninitTest(Object *thiz);
