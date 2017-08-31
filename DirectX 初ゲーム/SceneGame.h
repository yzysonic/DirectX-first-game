#pragma once
#include"Scene.h"
#include"Player.h"

Scene* GetSceneGame(void);
void initSceneGameBase(void);
void initSceneGame(void);
void updateSceneGame(void);
void uninitSceneGame(void);

Player* GetPlayer(void);