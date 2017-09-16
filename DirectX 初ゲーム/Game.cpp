#include "Game.h"
#include <windows.h>
#include "Direct3D.h"
#include "Input.h"
#include "Time.h"
#include "Texture.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "Physics.h"
#include "GameManager.h"

// プロトタイプ宣言
void CheckWinMesg();

// グローバル変数
bool g_bRunGame = true;
MSG g_Msg;


// ゲーム初期化
void InitGame(void)
{
	Time::Init();
	InitTexture();
	ObjectManager::Create();
	Renderer::Create();
	Physics::Create();
	GameManager::Create();
}

// ゲームループ
void RunGame(void)
{
	Renderer* renderer			= Renderer::GetInstance();
	Physics* physics			= Physics::GetInstance();
	ObjectManager* objManager	= ObjectManager::GetInstance();

	while (g_bRunGame)
	{
		CheckWinMesg();
		UpdateInput();
		GameManager::Update();
		objManager->update();
		physics->update();
		renderer->drawFrame();
		Time::FramerateControl();
	}
}

// ゲーム終了処理
void UninitGame(void)
{
	GameManager::Destroy();
	Physics::Destroy();
	Renderer::Destroy();
	ObjectManager::Destroy();
	UninitTexture();
	Time::Uninit();
}

// ゲーム終了
void StopGame(void)
{
	DestroyWindow(GetHWnd());	// ウィンドウを破棄するよう指示する
}

// ウィンドウズメッセージ処理
void CheckWinMesg()
{
	if (PeekMessage(&g_Msg, NULL, 0, 0, PM_REMOVE))
	{
		// PostQuitMessage()が呼ばれたらゲーム終了
		if (g_Msg.message == WM_QUIT)
		{
			g_bRunGame = false;
			return;
		}
		// メッセージの翻訳とディスパッチ
		else
		{
			TranslateMessage(&g_Msg);
			DispatchMessage(&g_Msg);
		}
	}
}

