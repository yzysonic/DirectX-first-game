#pragma once
#include "Scene.h"

void InitGame(Scene* startScene);
void RunGame(void);
void UninitGame(void);
bool EndGame(void);
void StopGame(void);
void SetGlobalUpdate(void(*update)(void));