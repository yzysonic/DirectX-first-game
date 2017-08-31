#include"SceneGame.h"

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
	g_SceneGame.player = newPlayer();
}

void updateSceneGame(void)
{
}

void uninitSceneGame(void)
{
	deletePlayer(g_SceneGame.player);
}

Player * GetPlayer(void)
{
	return g_SceneGame.player;
}
