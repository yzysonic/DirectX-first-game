//=============================================================================
//
// ファイル名：main.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "Core/System.h"
#include "Core/Window.h"
#include "Core/Game.h"
#include "Core/GameManager.h"
#include "SceneGlobal.h"
#include "SceneTitle.h"
#ifdef _DEBUG
#include "SceneGame.h"
#include "SceneTest.h"
#endif

// メモリリークの自動検出
#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // メモリリーク自動検出
	SystemParameters::FPS = 70;
#endif

	SystemParameters::windowTitle = "ヒットポリゴン";

	// 初期化
	if (FAILED(InitSystem()))
		return 1;

	// マウスを隠す
	ShowCursor(false);

#ifndef _DEBUG
	Window::SetWindowMode(false);
#endif

	// ゲーム初期化
	InitGame();
	GameManager::GetInstance()->SetGlobalScene(new SceneGlobal);
#ifndef _DEBUG
	GameManager::GetInstance()->SetScene(new SceneTitle);
#else
	GameManager::GetInstance()->SetScene(new SceneTitle);
#endif

	// ゲームループ
	RunGame();

	// 終了
	UninitGame();
	UninitSystem();

	return 0;
}
