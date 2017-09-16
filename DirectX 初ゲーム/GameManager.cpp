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

void GameManager::Create(void)
{
	Singleton::Create();
	FadeScreen::Create();
	SceneTitle::Create();
	SceneGame::Create();
	SceneGameOver::Create();
	SceneClear::Create();
	SceneTest::Create();
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
	SceneTitle::Destroy();
	SceneGame::Destroy();
	SceneGameOver::Destroy();
	SceneClear::Destroy();
	SceneTest::Destroy();
	FadeScreen::Destroy();
	Singleton::Destroy();
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
		m_pInstance->currentScene = SceneTitle::GetInstance();
		break;

	case SceneName::GAME:
		m_pInstance->currentScene = SceneGame::GetInstance();
		break;

	case SceneName::GAMEOVER:
		m_pInstance->currentScene = SceneGameOver::GetInstance();
		break;

	case SceneName::CLEAR:
		m_pInstance->currentScene = SceneClear::GetInstance();
		break;

	case SceneName::TEST:
		m_pInstance->currentScene = SceneTest::GetInstance();
		break;

	}

	InitRandom();
	m_pInstance->currentScene->init();

}