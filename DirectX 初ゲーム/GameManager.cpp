#include "GameManager.h"
#include "Random.h"
#include "FadeScreen.h"

void GameManager::Create(void)
{
	Singleton<GameManager>::Create();
	InitRandom();
	FadeScreen::Create();
}

void GameManager::Update(void)
{
	m_pInstance->currentScene->update();
}

void GameManager::Destroy(void)
{
	m_pInstance->currentScene->uninit();
	FadeScreen::Destroy();
	Singleton<GameManager>::Destroy();
}

void GameManager::SetScene(Scene* scene)
{
	m_pInstance->currentScene->uninit();
	LoadScene(scene);
}

void GameManager::LoadScene(Scene * scene)
{
	m_pInstance->currentScene.reset(scene);
	m_pInstance->currentScene->init();

}
