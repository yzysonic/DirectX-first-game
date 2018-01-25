#include "GameManager.h"

void GameManager::Create(void)
{
	Singleton<GameManager>::Create();
}

void GameManager::Update(void)
{
	if(m_pInstance->scene[0] != nullptr)
		m_pInstance->scene[0]->Update();

	if(m_pInstance->scene[1] != nullptr)
		m_pInstance->scene[1]->Update();
}



void GameManager::Destroy(void)
{
	for (auto& scene : m_pInstance->scene)
	{
		if (scene != nullptr)
			scene->Uninit();
	}

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

Scene * GameManager::GetScene(void)
{
	return m_pInstance->scene[1].get();
}

void GameManager::SetScene(Scene * scene, int no)
{
	if(m_pInstance->scene[no] != nullptr)
		m_pInstance->scene[no]->Uninit();

	m_pInstance->scene[no].reset(scene);

	if (scene != nullptr)
		m_pInstance->scene[no]->Init();
}
