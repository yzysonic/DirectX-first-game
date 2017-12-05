#include "SceneGuide.h"
#include "SceneGame.h"
#include "FadeScreen.h"

void SceneGuide::init(void)
{
	this->bk = new Object2D;
	this->bk->setPolygon(Layer::BG_00, Texture::Get("guide"), RendererType::UI);
	this->state = 0;
	this->timer = 0;
	
	FadeScreen::FadeIn(Color::white);
}

void SceneGuide::update(void)
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

void SceneGuide::uninit(void)
{
	delete this->bk;
}
