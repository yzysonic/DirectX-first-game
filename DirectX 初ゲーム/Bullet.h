#pragma once
#include "Core.h"

#define BULLET_MAX (100)

typedef struct _Bullet
{
	Object* base;
	float timer;
	int index;
}Bullet;

Bullet* newBullet(const Transform* transform, Vector3 velocity);
void initBullet(Object* thiz);
void updateBullet(Object* thiz);
void uninitBullet(Object* thiz);
void onCollisionBullet(Object* thiz, Object* other);
void CleanBullets();