//=============================================================================
//
// ファイル名：main.cpp
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#include "System.h"
#include "Game.h"
#include "GameManager.h"
#include "SceneGlobal.h"
#include "SceneTitle.h"

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// 初期化
	if (FAILED(InitSystem()))
		return 1;
	
	// ゲーム初期化
	InitGame();
	GameManager::GetInstance()->SetGlobalScene(new SceneGlobal);
	GameManager::GetInstance()->SetScene(new SceneTitle);

	// ゲームループ
	RunGame();

	// 終了
	UninitGame();
	UninitSystem();

	return 0;
}
