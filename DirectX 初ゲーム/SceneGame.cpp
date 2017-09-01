#include "SceneGame.h"
#include "test.h"
#include "Core.h"


typedef struct _SceneGame
{
	Scene base;
	Player* player = NULL;
}SceneGame;

SceneGame g_SceneGame;

Scene * GetSceneGame(void)
{
	return (Scene*)&g_SceneGame;
}

void initSceneGame(void)
{
	SetNewObj(g_SceneGame.player, Player);
}

void updateSceneGame(void)
{
	
}

void uninitSceneGame(void)
{
	DeleteObj(g_SceneGame.player);
}