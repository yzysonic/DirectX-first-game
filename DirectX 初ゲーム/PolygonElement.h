#pragma once
#include "Core.h"

typedef struct _PolygonElement
{
	Object *base;
	float timer;
	float timerInterval;
	Vector3 speed;
	Vector3 prePos;
	Color nextColor;
	void(*update)(_PolygonElement*);
}PolygonElement;

void initPolygonElement(Object *thiz);
void updatePolygonElement(Object *thiz);
void uninitPolygonElement(Object *thiz);
