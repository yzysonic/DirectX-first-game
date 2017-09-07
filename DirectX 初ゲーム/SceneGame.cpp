#include "SceneGame.h"
#include "Core.h"
#include "Camera.h"


typedef struct _SceneGame
{
	Scene base;
	Camera* camera = NULL;
	Player* player = NULL;
}SceneGame;

SceneGame g_SceneGame;

Scene * GetSceneGame(void)
{
	return (Scene*)&g_SceneGame;
}

void initSceneGame(void)
{
	g_SceneGame.camera = NewSubObj(Camera);
	g_SceneGame.player = NewSubObj(Player);
}

void updateSceneGame(void)
{
	
}

void uninitSceneGame(void)
{
	DeleteSubObj(g_SceneGame.camera);
	DeleteSubObj(g_SceneGame.player);
}