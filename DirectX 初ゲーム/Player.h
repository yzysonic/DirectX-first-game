#pragma once

#include"Object.h"

typedef struct _Player
{
	Object *base;
}Player;

Player* newPlayer(void);
void deletePlayer(Player *thiz);
void initPlayer(Object *thiz);
void updatePlayer(Object *thiz);
void uninitPlayer(Object *thiz);