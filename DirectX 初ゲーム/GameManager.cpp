#include "GameManager.h"
#include "main.h"
#include "Random.h"
#include "Renderer.h"
#include "FadeScreen.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneGameOver.h"
#include "SceneClear.h"
#include "SceneTest.h"

Scene* g_CurrentScene;

void LoadScene(SceneName scene);

void InitGameManager(void)
{
	InitFadeScreen();
	LoadScene(START_SCENE);
}

void UpdateGameManager(void)
{
	g_CurrentScene->update();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_1))
		SetScene(SCENE_TITLE);
	if (GetKeyboardTrigger(DIK_2))
		SetScene(SCENE_GAME);
	if (GetKeyboardTrigger(DIK_3))
		SetScene(SCENE_GAMEOVER);
	if (GetKeyboardTrigger(DIK_4))
		SetScene(SCENE_CLEAR);
	if (GetKeyboardTrigger(DIK_0))
		SetScene(SCENE_TEST);
#endif
}

void UninitGameManager(void)
{
	g_CurrentScene->uninit();
	UninitFadeScreen();
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
		g_CurrentScene = GetSceneTitle();
		g_CurrentScene->init = &initSceneTitle;
		g_CurrentScene->update = &updateSceneTitle;
		g_CurrentScene->uninit = &uninitSceneTitle;
		break;

	case SCENE_GAME:
		g_CurrentScene = GetSceneGame();
		g_CurrentScene->init = &initSceneGame;
		g_CurrentScene->update = &updateSceneGame;
		g_CurrentScene->uninit = &uninitSceneGame;
		break;

	case SCENE_GAMEOVER:
		g_CurrentScene = GetSceneGameOver();
		g_CurrentScene->init = &initSceneGameOver;
		g_CurrentScene->update = &updateSceneGameOver;
		g_CurrentScene->uninit = &uninitSceneGameOver;
		break;

	case SCENE_CLEAR:
		g_CurrentScene = GetSceneClear();
		g_CurrentScene->init = &initSceneClear;
		g_CurrentScene->update = &updateSceneClear;
		g_CurrentScene->uninit = &uninitSceneClear;
		break;

	case SCENE_TEST:
		g_CurrentScene = GetSceneTest();
		g_CurrentScene->init = &initSceneTest;
		g_CurrentScene->update = &updateSceneTest;
		g_CurrentScene->uninit = &uninitSceneTest;
		break;

	}

	InitRandom();

	if (g_CurrentScene->init != NULL)
		g_CurrentScene->init();

}