#pragma once
#include "Scene.h"

#ifdef _DEBUG
#define START_SCENE SCENE_GAME
#else
#define START_SCENE SCENE_TITLE
#endif

void InitGameManager(void);
void UpdateGameManager(void);
void UninitGameManager(void);
void SetScene(SceneName scene);