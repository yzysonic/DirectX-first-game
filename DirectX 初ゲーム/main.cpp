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

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

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
