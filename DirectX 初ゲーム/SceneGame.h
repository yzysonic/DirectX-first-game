#pragma once
#include "Scene.h"
#include "main.h"

#define FIELD_RANG_X (SCREEN_CENTER_X+500)
#define FIELD_RANG_Y (SCREEN_CENTER_Y+500)
#define ENEMY_MAX (10)
#define GAME_POLY_MAX (1000)

Scene* GetSceneGame(void);
void initSceneGame(void);
void updateSceneGame(void);
void uninitSceneGame(void);
int GetGameScore(void);
void AddGameScore(int score);