#include "Game.h"
#include "Window.h"
#include "Direct3D.h"
#include "Input.h"
#include "Time.h"
#include "Texture.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "Physics.h"
#include "GameManager.h"

// �O���[�o���ϐ�
bool g_bRunGame = true;
void(*g_pGlobalUpdate)(void) = [](void) {};

// �Q�[��������
void InitGame(Scene* startScene)
{
	Time::Init();
	InitTexture();
	ObjectManager::Create();
	Renderer::Create();
	Physics::Create();
	GameManager::Create();
	GameManager::GetInstance()->LoadScene(startScene);
}

// �Q�[�����[�v
void RunGame(void)
{
	while (g_bRunGame)
	{
		Window::CheckMesg();
		UpdateInput();
		g_pGlobalUpdate();
		GameManager::Update();
		Physics::Update();
		ObjectManager::Update();
		Renderer::DrawFrame();
		Time::FramerateControl();
	}
}

// �Q�[���I������
void UninitGame(void)
{
	GameManager::Destroy();
	Physics::Destroy();
	Renderer::Destroy();
	ObjectManager::Destroy();
	UninitTexture();
	Time::Uninit();
}

// �Q�[���I���m�F
bool EndGame(void)
{
	return true;
}

// �Q�[���I��
void StopGame(void)
{
	g_bRunGame = false;
}

void SetGlobalUpdate(void(*update)(void))
{
	if(update != nullptr)
		g_pGlobalUpdate = update;
	else
		g_pGlobalUpdate = [](void) {};
}
