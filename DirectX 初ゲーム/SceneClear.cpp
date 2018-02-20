#include "SceneClear.h"
#include "Core/Core.h"
#include "SceneTitle.h"
#include "FadeScreen.h"


void SceneClear::Init(void)
{
	this->title = new Object;
	this->title->transform.position.y = 0.3f*SystemParameters::ResolutionY;
	this->title->AddComponent<RectPolygon2D>("result_title", Layer::DEFAULT);

	this->score = new NumberUI(5, -120, 0, "number", "game_score");
	this->score->setOffset(130, 0);
	this->score->setNumber(GameManager::Var<int>("score"));

	Renderer::GetInstance()->getCamera("default")->setBackColor(180, 180, 180, 255);

	this->timer = 0;

	// BGMを再生
	SetVolume(BGM_CLEAR, -1800);
	PlayBGM(BGM_CLEAR);

	// フェイトイン効果
	FadeScreen::FadeIn(Color::white);

	SceneClear::pUpdate = &SceneClear::update_clear_fadeWait;
}

void SceneClear::Update(void)
{
	(this->*pUpdate)();
	this->timer += Time::DeltaTime();
}

void SceneClear::Uninit(void)
{
	StopSound(BGM_CLEAR);
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
		FadeScreen::FadeOut(Color::black, 0.7f);
		this->timer = 0;
		SceneClear::pUpdate = &SceneClear::update_clear_state1;
	}
		
}

void SceneClear::update_clear_state1(void)
{
	if (this->timer > 1.0f)
	{
		GameManager::SetScene(new SceneTitle);
	}
		
}
