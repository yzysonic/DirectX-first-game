#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "FadeScreen.h"

void SceneGameOver::init(void)
{
	this->bk = new Object2D;
	this->bk->setPolygon(Layer::BG_00, TEX_GAME_OVER, RendererType::UI);
	this->timer = 0;

	// BGMを再生
	SetVolume(BGM_GAMEOVER, -1800);
	PlayBGM(BGM_GAMEOVER);

	// フェイトイン効果
	FadeScreen::FadeIn(Color::black);

	SceneGameOver::pUpdate = &SceneGameOver::update_fadeWait;
}

void SceneGameOver::update(void)
{
	(this->*pUpdate)();
	this->timer += Time::DeltaTime();
}

void SceneGameOver::uninit(void)
{
	StopSound(BGM_GAMEOVER);
	delete this->bk;
}

void SceneGameOver::update_fadeWait(void)
{
	if (FadeScreen::Finished())
	{
		SceneGameOver::pUpdate = &SceneGameOver::update_state0;
	}
}

void SceneGameOver::update_state0(void)
{
	if (this->timer > 4.0f)
	{
		FadeScreen::FadeOut(Color::black, 0.7f);
		this->timer = 0;
		SceneGameOver::pUpdate = &SceneGameOver::update_state1;
	}
		
}

void SceneGameOver::update_state1(void)
{
	if (this->timer > 1.0f)
	{
		GameManager::SetScene(new SceneTitle);
	}
}
