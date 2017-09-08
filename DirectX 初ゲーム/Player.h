#pragma once

#include"Core.h"
#include "Bullet.h"


typedef struct _Player
{
	Object *base;
	Vector3 dir;
	int lives;
	float speed;
	float timer;
}Player;

void initPlayer(Object *thiz);
void updatePlayer(Object *thiz);
void uninitPlayer(Object *thiz);
void onCollisionPlayer(Object *thiz, Object *other);