#pragma once

#include"Core.h"

typedef struct _Player
{
	Object *base;
}Player;

void initPlayer(Object *thiz);
void updatePlayer(Object *thiz);
void uninitPlayer(Object *thiz);
void onCollisionPlayer(Object *thiz, Object *other);