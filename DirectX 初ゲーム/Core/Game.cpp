#include "Game.h"
#include "Window.h"
#include "Direct3D.h"
#include "Input.h"
#include "Time.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "Physics.h"
#include "GameManager.h"
#include "Random.h"
#include "Texture.h"

// �O���[�o���ϐ�
bool g_bRunGame = true;

// �Q�[��������
void InitGame()
{
	Time::Init();
	ObjectManager::Create();
	GameManager::Create();
	Renderer::Create();
	Physics::Create();
	InitRandom();
	Texture::Init();
}

// �Q�[�����[�v
void RunGame(void)
{
	while (g_bRunGame)
	{
		Window::CheckMesg();
		UpdateInput();
		Physics::Update();
		GameManager::Update();
		ObjectManager::Update();
		Renderer::DrawFrame();
		Time::FramerateControl();
	}
}

// �Q�[���I������
void UninitGame(void)
{
	GameManager::Destroy();
	ObjectManager::Destroy();
	Texture::Uninit();
	Physics::Destroy();
	Renderer::Destroy();
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