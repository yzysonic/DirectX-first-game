#include "GameManager.h"
#include "ObjectManager.h"

void GameManager::Create(void)
{
	Singleton<GameManager>::Create();
	m_pInstance->scene_stack_num = 0;
}

void GameManager::Update(void)
{
	if (m_pInstance->scene[0] != nullptr)
	{
		m_pInstance->scene[0]->Update();
		m_pInstance->scene[0]->UpdateObjects();
	}

	if (m_pInstance->scene[m_pInstance->scene_stack_num + 1] != nullptr)
	{
		m_pInstance->scene[m_pInstance->scene_stack_num + 1]->Update();
		m_pInstance->scene[m_pInstance->scene_stack_num + 1]->UpdateObjects();
	}
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
	ClearSceneStack();
	SetScene(scene, 1);
}

void GameManager::PushScene(Scene * scene)
{
	UINT top = m_pInstance->scene_stack_num + 1;
	if (top > SceneStackMax)
		return;

	m_pInstance->scene[top]->OnPause();
	m_pInstance->scene[top]->PauseObjects();
	m_pInstance->scene[top + 1].reset(scene);
	m_pInstance->scene_stack_num++;
	scene->Init();
}

void GameManager::PopScene(void)
{
	if (m_pInstance->scene_stack_num == 0)
		return;

	m_pInstance->scene[m_pInstance->scene_stack_num + 1]->Uninit();
	m_pInstance->scene[m_pInstance->scene_stack_num + 1].reset();
	m_pInstance->scene[m_pInstance->scene_stack_num]->ResumeObjects();
	m_pInstance->scene[m_pInstance->scene_stack_num]->OnResume();
	m_pInstance->scene_stack_num--;
}

Scene * GameManager::GetScene(void)
{
	return m_pInstance->scene[m_pInstance->scene_stack_num + 1].get();
}

Scene * GameManager::GetGlobalScene(void)
{
	return m_pInstance->scene[0].get();
}

void GameManager::SetScene(Scene * scene, int no)
{

	if(m_pInstance->scene[no] != nullptr)
		m_pInstance->scene[no]->Uninit();

	m_pInstance->scene[no].reset(scene);
	ObjectManager::KillObject();

	if (scene != nullptr)
		m_pInstance->scene[no]->Init();
}

void GameManager::ClearSceneStack(void)
{
	for (UINT i = 0; i < m_pInstance->scene_stack_num; i++)
		PopScene();
}
