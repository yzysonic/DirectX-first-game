#pragma once
#include "Core.h"

typedef struct _Bullet
{
	Object* base;
	float timer;
}Bullet;

void initBullet(Object* thiz);
void updateBullet(Object* thiz);
void uninitBullet(Object* thiz);