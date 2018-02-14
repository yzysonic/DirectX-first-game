#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "FadeScreen.h"

void SceneGameOver::Init(void)
{
	this->text = new Object;
	this->text->AddComponent<RectPolygon2D>("game_over");
	this->timer = 0;

	Renderer::GetInstance()->getCamera("default")->setBackColor(0, 0, 0, 255);

	// BGMを再生
	SetVolume(BGM_GAMEOVER, -1800);
	PlayBGM(BGM_GAMEOVER);

	// フェイトイン効果
	FadeScreen::FadeIn(Color::black);

	SceneGameOver::pUpdate = &SceneGameOver::update_fadeWait;
}

void SceneGameOver::Update(void)
{
	(this->*pUpdate)();
	this->timer += Time::DeltaTime();
}

void SceneGameOver::Uninit(void)
{
	StopSound(BGM_GAMEOVER);
	delete this->text;
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
