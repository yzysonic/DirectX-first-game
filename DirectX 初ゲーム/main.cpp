//=============================================================================
//
// ファイル名：main.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "Core/System.h"
#include "Core/Game.h"
#include "Core/GameManager.h"
#include "SceneGlobal.h"
#include "SceneTitle.h"

// メモリリークの自動検出
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
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
#endif

	SystemParameters::windowTitle = "ヒットポリゴン";

	// 初期化
	if (FAILED(InitSystem()))
		return 1;

	// マウスを隠す
	//ShowCursor(false);

	// ゲーム初期化
	InitGame();
	SceneGlobal::Create();
	GameManager::GetInstance()->SetGlobalScene(SceneGlobal::GetInstance());
	GameManager::GetInstance()->SetScene(new SceneTitle);

	// ゲームループ
	RunGame();

	// 終了
	UninitGame();
	UninitSystem();

	return 0;
}
