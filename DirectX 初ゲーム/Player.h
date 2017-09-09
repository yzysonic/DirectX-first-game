#pragma once

#include"Core.h"
#include "Bullet.h"


typedef struct _Player
{
	Object *base;
	Vector3 dir;
	int hp;
	float speed;
	float timer;
	float timer2;
	bool muteki;
}Player;

void initPlayer(Object *thiz);
void updatePlayer(Object *thiz);
void uninitPlayer(Object *thiz);
void onCollisionPlayer(Object *thiz, Object *other);