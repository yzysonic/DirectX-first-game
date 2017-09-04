#pragma once

#include"Core.h"
#include "Bullet.h"

#define BULLET_MAX (30)

typedef struct _Player
{
	Object *base;
	Bullet *bulletWk[BULLET_MAX];
	Vector3 dir;
	float speed;
	float timer;
}Player;

void initPlayer(Object *thiz);
void updatePlayer(Object *thiz);
void uninitPlayer(Object *thiz);
void onCollisionPlayer(Object *thiz, Object *other);