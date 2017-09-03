#include "GameManager.h"
#include "main.h"
#include "Random.h"
#include "Renderer.h"
#include "SceneGame.h"
#include "SceneTest.h"

Scene* g_CurrentScene;

void LoadScene(SceneName scene);

void InitGameManager(void)
{
	LoadScene(SCENE_TEST);
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
	if (g_CurrentScene->uninit != NULL)
		g_CurrentScene->uninit();

	LoadScene(scene);
}

void LoadScene(SceneName scene)
{
	switch (scene)
	{
	case SCENE_TITLE:

		break;
	case SCENE_GAME:
		g_CurrentScene = GetSceneGame();
		g_CurrentScene->init = &initSceneGame;
		g_CurrentScene->update = &updateSceneGame;
		g_CurrentScene->uninit = &uninitSceneGame;
		break;
	case SCENE_RESULT:

		break;
	case SCENE_TEST:
		g_CurrentScene = GetSceneTest();
		g_CurrentScene->init = &initSceneTest;
		g_CurrentScene->update = &updateSceneTest;
		g_CurrentScene->uninit = &uninitSceneTest;
		break;
	}

	InitRandom();
	Renderer_SetCamera(NULL);

	if (g_CurrentScene->init != NULL)
		g_CurrentScene->init();

}