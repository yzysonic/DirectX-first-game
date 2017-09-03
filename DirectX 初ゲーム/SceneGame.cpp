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
	g_SceneGame.camera = NewObj(Camera);
	g_SceneGame.player = NewObj(Player);
}

void updateSceneGame(void)
{
	
}

void uninitSceneGame(void)
{
	DeleteObj(g_SceneGame.camera);
	DeleteObj(g_SceneGame.player);
}