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

		this->menu->GetComponent<RectPolygon2D>()->SetOpacity(progress*0.6f);
		this->menu->title->GetComponent<RectPolygon2D>()->SetOpacity(progress);
		this->menu->cursor->GetComponent<RectPolygon2D>()->SetOpacity(progress);
		this->menu->item0->GetComponent<RectPolygon2D>()->SetOpacity(progress);
		this->menu->item1->GetComponent<RectPolygon2D>()->SetOpacity(progress);

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

	if (IsButtonTriggered(0, BUTTON_OPTIONS) || IsButtonTriggered(0, BUTTON_PAD) || IsButtonTriggered(0, BUTTON_CR))
	{
		this->exit = true;
		this->timer.Reset(0.15f);
	}

	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_CI))
	{
		if (this->menu->GetCursor() == 0)
		{
			this->exit = true;
			this->timer.Reset(0.15f);
		}
		else if (this->menu->GetCursor() == 1)
		{
			GameManager::Var<PostEffect*>("post_effect")->SetBlurActive(false);
			GameManager::SetScene(new SceneTitle);
		}
		
	}
}

PauseMenu::PauseMenu(void)
{
	RectPolygon2D* poly;

	poly = AddComponent<RectPolygon2D>("none", Layer::TOP, "post");
	poly->setSize((float)SystemParameters::ResolutionX, (float)SystemParameters::ResolutionY);
	poly->SetColor(Color(72, 72, 72, 255));
	poly->SetOpacity(0.0f);

	this->title = new Object;
	this->title->transform.position.y = 160.0f;
	this->title->AddComponent<RectPolygon2D>("pause_title", Layer::TOP, "post")->SetOpacity(0.0f);

	this->cursor = new Object;
	this->cursor->transform.position.x = -130.0f;
	this->cursor->transform.position.y = ItemOffset;
	this->cursor->AddComponent<RectPolygon2D>("title_cursor", Layer::TOP, "post")->SetOpacity(0.0f);

	this->item0 = new Object;
	this->item0->transform.position.y = ItemOffset;
	this->item0->AddComponent<RectPolygon2D>("pause_item0", Layer::TOP, "post")->SetOpacity(0.0f);

	this->item1 = new Object;
	this->item1->transform.position.y = ItemOffset-ItemMargin;
	this->item1->AddComponent<RectPolygon2D>("pause_item1", Layer::TOP, "post")->SetOpacity(0.0f);

	this->item0->transform.position.x = 0.5f*(this->item0->GetComponent<RectPolygon2D>()->getSize().x - this->item1->GetComponent<RectPolygon2D>()->getSize().x);

	this->cursor_index = 0;
}

void PauseMenu::Update(void)
{
	if ((GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP)) && this->cursor_index > 0)
		SetCursor(this->cursor_index - 1);
	if ((GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN)) && this->cursor_index < 1)
		SetCursor(this->cursor_index + 1);

}

int PauseMenu::GetCursor(void)
{
	return this->cursor_index;
}

void PauseMenu::SetCursor(int index)
{
	this->cursor->transform.position.y = ItemOffset - ItemMargin*index;
	this->cursor_index = index;
}
