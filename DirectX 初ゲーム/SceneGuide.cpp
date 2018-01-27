#include "SceneGuide.h"
#include "SceneGame.h"
#include "FadeScreen.h"

void SceneGuide::Init(void)
{
	this->bk.reset(new Object);
	this->bk->AddComponent<RectPolygon2D>("guide", Layer::BG_00);
	this->state = 0;
	this->timer = 0;
	
	FadeScreen::FadeIn(Color::white);
}

void SceneGuide::Update(void)
{
	switch (this->state)
	{
	case 0:
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_CI))
		{
			this->state = 1;
			FadeScreen::FadeOut(Color::white);
		}
		break;
	case 1:
		if (FadeScreen::Finished())
		{
			GameManager::SetScene(new SceneGame);
		}
		break;
	}
}