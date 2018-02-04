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

// グローバル変数
bool g_bRunGame = true;

// ゲーム初期化
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

// ゲームループ
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

// ゲーム終了処理
void UninitGame(void)
{
	GameManager::Destroy();
	ObjectManager::Destroy();
	Texture::Uninit();
	Physics::Destroy();
	Renderer::Destroy();
	Time::Uninit();
}

// ゲーム終了確認
bool EndGame(void)
{
	return true;
}

// ゲーム終了
void StopGame(void)
{
	g_bRunGame = false;
}