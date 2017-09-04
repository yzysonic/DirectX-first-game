#pragma once

#include"Core.h"

typedef struct _Player
{
	Object *base;
	Vector3 dir;
	float speed;
}Player;

void initPlayer(Object *thiz);
void updatePlayer(Object *thiz);
void uninitPlayer(Object *thiz);
void onCollisionPlayer(Object *thiz, Object *other);