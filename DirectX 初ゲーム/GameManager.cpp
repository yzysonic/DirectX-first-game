#include "GameManager.h"
#include "FadeScreen.h"

void GameManager::Create(void)
{
	Singleton<GameManager>::Create();
}

void GameManager::Update(void)
{
	if(m_pInstance->scene[0] != nullptr)
		m_pInstance->scene[0]->update();

	if(m_pInstance->scene[1] != nullptr)
		m_pInstance->scene[1]->update();
}



void GameManager::Destroy(void)
{
	if (m_pInstance->scene[1] != nullptr)
		m_pInstance->scene[1]->uninit();

	if (m_pInstance->scene[0] != nullptr)
		m_pInstance->scene[0]->uninit();

	Singleton<GameManager>::Destroy();
}

void GameManager::SetGlobalScene(Scene * scene)
{
	SetScene(scene, 0);
}

void GameManager::SetScene(Scene* scene)
{
	SetScene(scene, 1);
}

void GameManager::SetScene(Scene * scene, int no)
{
	if(m_pInstance->scene[no] != nullptr)
		m_pInstance->scene[no]->uninit();

	m_pInstance->scene[no].reset(scene);

	if (scene != nullptr)
		m_pInstance->scene[no]->init();
}
