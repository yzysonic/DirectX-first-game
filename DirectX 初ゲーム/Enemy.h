#pragma once
#include "Core.h"

#define ENEMY_SPEED (50)

typedef struct _Enemy
{
	Object *base = NULL;
	const Transform *target = NULL;
	int hp;
}Enemy;


void initEnemy(Object* thiz);
void updateEnemy(Object* thiz);
void uninitEnemy(Object* thiz);
void onCollisionEnemy(Object* thiz, Object* other);