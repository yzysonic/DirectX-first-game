#include "SceneGlobal.h"
#include "Core/Core.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneGameOver.h"
#include "SceneClear.h"
#include "SceneTest.h"
#include "FadeScreen.h"


void SceneGlobal::init(void)
{
	FadeScreen::Create();
}

void SceneGlobal::update(void)
{
#ifdef _DEBUG
	GameManager* gm = GameManager::GetInstance();

	if (GetKeyboardTrigger(DIK_1))
		gm->SetScene(new SceneTitle);
	if (GetKeyboardTrigger(DIK_2))
		gm->SetScene(new SceneGame);
	if (GetKeyboardTrigger(DIK_3))
		gm->SetScene(new SceneGameOver);
	if (GetKeyboardTrigger(DIK_4))
		gm->SetScene(new SceneClear);
	if (GetKeyboardTrigger(DIK_0))
		gm->SetScene(new SceneTest);
#endif
}

void SceneGlobal::uninit(void)
{
	FadeScreen::Destroy();
}