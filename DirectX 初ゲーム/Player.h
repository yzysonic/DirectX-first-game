#pragma once

#include"Object.h"

typedef struct _Player
{
	Object *base;
}Player;

void initPlayer(Object *thiz);
void updatePlayer(Object *thiz);
void uninitPlayer(Object *thiz);