#include"GameManager.h"
#include"main.h"
#include"SceneGame.h"

Scene* g_CurrentScene;

void InitGameManager(void)
{
	g_CurrentScene = GetSceneGame();
	g_CurrentScene->init = &initSceneGame;
	g_CurrentScene->update = &updateSceneGame;
	g_CurrentScene->uninit = &uninitSceneGame;

	g_CurrentScene->init();
}

void UpdateGameManager(void)
{
	g_CurrentScene->update();
}

void UninitGameManager(void)
{
	g_CurrentScene->uninit();
}

void SetScene(SceneName scene)
{
	g_CurrentScene->uninit();

	switch (scene)
	{
	case SCENE_TITLE:

		break;
	case SCENE_GAME:
		g_CurrentScene = GetSceneGame();
		break;
	case SCENE_RESULT:

		break;
	}

	g_CurrentScene->init();
}
