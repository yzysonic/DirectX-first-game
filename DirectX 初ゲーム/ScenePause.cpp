#include "ScenePause.h"
#include "SceneTitle.h"
#include "PostEffect.h"

void ScenePause::Init(void)
{
	this->menu = new PauseMenu;
	this->timer.Reset(0.3f);
	this->exit = false;
	GameManager::Var<PostEffect*>("post_effect")->SetBlurActive(true);
}

void ScenePause::Update(void)
{
	if (!this->timer.TimeUp())
	{
		float progress = Ease::CircOut(0.0f, 1.0f, this->timer.Progress());

		if (this->exit)
			progress = 1.0f - progress;

		this->menu->GetComponent<RectPolygon2D>()->SetOpacity(progress);
		GameManager::Var<PostEffect*>("post_effect")->SetBlur(progress*100.0f);

		this->timer++;
	}
	else
	{
		if (this->exit)
		{
			GameManager::Var<PostEffect*>("post_effect")->SetBlurActive(false);
			GameManager::PopScene();
		}
	}

	if (GetKeyboardPress(DIK_1) || GetKeyboardTrigger(DIK_RETURN))
	{
		this->exit = true;
		this->timer.Reset(0.15f);
	}
	if (GetKeyboardPress(DIK_2))
	{
		GameManager::SetScene(new SceneTitle);
	}
}

void PauseMenu::OnDraw(void)
{
	//Direct3D::GetDevice()->SetPixelShader(this->shader->pD3DShader);
}

void PauseMenu::AfterDraw(void)
{
	//Direct3D::GetDevice()->SetPixelShader(NULL);
}

PauseMenu::PauseMenu(void)
{
	AddComponent<RectPolygon2D>("pause", Layer::TOP, "post")->SetOpacity(0.0f);

	//this->shader = PixelShader::Get("GaussianBlurPS");
}
