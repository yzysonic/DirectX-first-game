#include "SceneClear.h"
#include "GameManager.h"
#include "Core.h"
#include "SceneGame.h"


void SceneClear::init(void)
{
	this->bk = new Object;
	this->bk->setPolygon(Layer::BG_00, TEX_CLEAR, RendererType::UI);

	this->score = new NumberUI(5, -120, 0, TEX_NUMBER, TEX_GAME_SCORE);
	this->score->setOffset(130, 0);
	this->score->setNumber(GameManager::GetInstance()->score);

	this->timer = 0;

	// BGMを再生
	SetVolume(BGM_CLEAR, -1800);
	PlayBGM(BGM_CLEAR);

	// フェイトイン効果
	FadeScreen(FADE_IN_WH);

	SceneClear::pUpdate = &SceneClear::update_clear_fadeWait;
}

void SceneClear::update(void)
{
	(this->*pUpdate)();
	this->timer += Time::DeltaTime();
}

void SceneClear::uninit(void)
{
	StopSound(BGM_CLEAR);
	delete this->score;
	delete this->bk;
}

void SceneClear::update_clear_fadeWait(void)
{
	if (FadeScreen::Finished())
	{
		SceneClear::pUpdate = &SceneClear::update_clear_state0;
	}
}

void SceneClear::update_clear_state0(void)
{
	if (this->timer > 4.0f)
	{
		FadeScreen::Fade(FADE_OUT_BK, 0, 0.7f);
		this->timer = 0;
		SceneClear::pUpdate = &SceneClear::update_clear_state1;
	}
		
}

void SceneClear::update_clear_state1(void)
{
	if (this->timer > 1.0f)
	{
		GameManager::SetScene(SceneName::TITLE);
	}
		
}
