#include "SceneGameOver.h"
#include "GameManager.h"


void SceneGameOver::init(void)
{
	this->bk = new Object;
	this->bk->setPolygon(Layer::BG_00, TEX_GAME_OVER, RendererType::UI);
	this->timer = 0;

	// BGMを再生
	SetVolume(BGM_GAMEOVER, -1800);
	PlayBGM(BGM_GAMEOVER);

	// フェイトイン効果
	FadeScreen::Fade(FADE_IN_WH);

	SceneGameOver::pUpdate = &SceneGameOver::update_fadeWait;
}

void SceneGameOver::update(void)
{
	(this->*pUpdate)();
	this->timer += Time::DeltaTime();
}

void SceneGameOver::uninit(void)
{
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
		FadeScreen::Fade(FADE_OUT_BK, 0, 0.7f);
		this->timer = 0;
		SceneGameOver::pUpdate = &SceneGameOver::update_state1;
	}
		
}

void SceneGameOver::update_state1(void)
{
	if (this->timer > 1.0f)
	{
		StopSound(BGM_GAMEOVER);
		GameManager::SetScene(SceneName::TITLE);
	}
}
