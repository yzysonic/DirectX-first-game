#include "ScenePause.h"
#include "SceneTitle.h"

void ScenePause::Init(void)
{
	//AddObject(new Object)
	//	->AddComponent<RectPolygon2D>("pause", Layer::TOP);
}

void ScenePause::Update(void)
{
	if (GetKeyboardPress(DIK_1) || GetKeyboardPress(DIK_PAUSE))
	{
		GameManager::PopScene();
	}
	if (GetKeyboardPress(DIK_2))
	{
		GameManager::SetScene(new SceneTitle);
	}
}
