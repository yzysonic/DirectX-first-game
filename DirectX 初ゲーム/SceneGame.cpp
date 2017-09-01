#include "SceneGame.h"
#include "test.h"
#include "Time.h"

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
	static float timer = 0;

	timer += GetDeltaTime();

	if (timer > 0.7f)
	{
		newObject(ObjType_Test);
		timer = 0;
	}
		
}

void uninitSceneGame(void)
{
	deletePlayer(g_SceneGame.player);
}

Player * GetPlayer(void)
{
	return g_SceneGame.player;
}
