#include "GameManager.h"
#include "main.h"
#include "Random.h"
#include "Renderer.h"
#include "FadeScreen.h"
#include "SceneTitle.h"
#include "SceneGuide.h"
#include "SceneGame.h"
#include "SceneGameOver.h"
#include "SceneClear.h"
#include "SceneTest.h"

void GameManager::Create(void)
{
	Singleton<GameManager>::Create();
	InitRandom();
	FadeScreen::Create();

	m_pInstance->score = 0;

	LoadScene(START_SCENE);
}

void GameManager::Update(void)
{
	m_pInstance->currentScene->update();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_1))
		SetScene(SceneName::TITLE);
	if (GetKeyboardTrigger(DIK_2))
		SetScene(SceneName::GAME);
	if (GetKeyboardTrigger(DIK_3))
		SetScene(SceneName::GAMEOVER);
	if (GetKeyboardTrigger(DIK_4))
		SetScene(SceneName::CLEAR);
	if (GetKeyboardTrigger(DIK_0))
		SetScene(SceneName::TEST);
#endif
}

void GameManager::Destroy(void)
{
	m_pInstance->currentScene->uninit();
	FadeScreen::Destroy();
	Singleton<GameManager>::Destroy();
}

void GameManager::SetScene(SceneName scene)
{
	m_pInstance->currentScene->uninit();
	LoadScene(scene);
}

void GameManager::LoadScene(SceneName scene)
{
	switch (scene)
	{
	case SceneName::TITLE:
		m_pInstance->currentScene.reset(new SceneTitle);
		break;

	case SceneName::GUIDE:
		m_pInstance->currentScene.reset(new SceneGuide);
		break;

	case SceneName::GAME:
		m_pInstance->currentScene.reset(new SceneGame);
		break;

	case SceneName::GAMEOVER:
		m_pInstance->currentScene.reset(new SceneGameOver);
		break;

	case SceneName::CLEAR:
		m_pInstance->currentScene.reset(new SceneClear);
		break;

	case SceneName::TEST:
		m_pInstance->currentScene.reset(new SceneTest);
		break;

	}

	m_pInstance->currentScene->init();

}