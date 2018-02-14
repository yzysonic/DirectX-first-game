#include "SceneTitle.h"
#include "Core\Game.h"
#include "SceneGuide.h"
#include "FadeScreen.h"
#include "CameraFov.h"


// 初期化
void SceneTitle::Init(void)
{
	this->beat = new Timer();
	this->beatCount = 0;
	this->timer = 0;
	this->com = COM_WAIT;
	this->playBack = false;
	this->cursorPos = 0;

	this->vignetting = (new Object);
	this->vignetting->AddComponent<RectPolygon2D>("vignetting", Layer::UI_02);

	this->logo = (new Object);
	this->logo->AddComponent<RectPolygon>("title_logo", Layer::UI_00);
	this->logo->transform.position = Vector3(0, SystemParameters::ResolutionY / 2.f - 200, 0);
	this->logo->SetActive(false);

	this->presskey = (new Object);
	this->presskey->AddComponent<RectPolygon2D>("title_presskey", Layer::UI_00);
	this->presskey->transform.position = Vector3(0, SystemParameters::ResolutionY / 2.f - 475, 0);
	this->presskey->SetActive(false);

	this->info = (new Object);
	this->info->AddComponent<RectPolygon2D>("title_info", Layer::UI_02);
	this->info->transform.position.x = SystemParameters::ResolutionX / 2 - this->info->GetComponent<RectPolygon2D>()->pTexture->size.x / 2 - 30;
	this->info->transform.position.y = -SystemParameters::ResolutionY / 2 + this->info->GetComponent<RectPolygon2D>()->pTexture->size.y + 10;
	this->info->transform.position.z = 0;
	this->info->SetActive(false);

	this->cursor = (new Object);
	this->cursor->AddComponent<RectPolygon>("title_cursor", Layer::UI_00);
	this->cursor->GetComponent<RectPolygon>()->SetOpacity(0);
	this->cursor->transform.position = Vector3(-90, SystemParameters::ResolutionY / 2.f - 415, 0);
	this->cursor->SetActive(false);

	this->start = (new Object);
	this->start->AddComponent<RectPolygon>("title_start", Layer::UI_00);
	this->start->GetComponent<RectPolygon>()->SetOpacity(0);
	this->start->transform.position = Vector3(40, SystemParameters::ResolutionY / 2.f - 415, 0);
	this->start->SetActive(false);
	
	this->exit = (new Object);
	this->exit->AddComponent<RectPolygon>("title_exit", Layer::UI_00);
	this->exit->GetComponent<RectPolygon>()->SetOpacity(0);
	this->exit->transform.position = Vector3(40 + 0.5f*(this->exit->GetComponent<RectPolygon>()->GetSize().x - this->start->GetComponent<RectPolygon>()->GetSize().x), SystemParameters::ResolutionY / 2.f - 500, 0);
	this->exit->SetActive(false);

	// アニメーション状態初期化
	this->logoState =
	this->keyState =
	this->polyState = Wait;

	// カメラの初期化
	this->camera = (new TitleCamera);

	// 背景ポリゴンの初期化
	this->field_poly = new FieldPolygon(TitlePolyMax/6);
	this->title_poly = new TitlePoly(TitlePolyMax*5/6);
	this->field_poly->SetActive(false);

	// フェイトアウト
	FadeScreen::FadeOut(Color::black, 0);

	// 初期状態
	SceneTitle::pUpdate = &SceneTitle::update_fadeWait;
}

// グローバル更新処理
void SceneTitle::Update(void)
{
	// 各アニメーション処理
	update_anime_poly();
	update_anime_logo();

	// ローカル更新処理
	(this->*pUpdate)();

	this->timer += Time::DeltaTime();
}

// 終了処理
void SceneTitle::Uninit(void)
{
	SafeDelete(this->beat);
	StopSound(BGM_TITLE);
}


// アニメーションとBGMの同期処理
void SceneTitle::syncAnimation(void)
{
	if (this->beat->elapsedTime() >= (60.0f / TitleBGMBPS)*(this->beatCount + 1))
	{
		while (this->beat->elapsedTime() >= (60.0f / TitleBGMBPS)*(this->beatCount + 1))
			this->beatCount++;

		if ((this->beatCount % 2) == 0)
			this->logoState = Hit;
		if ((this->beatCount % 2) == 0)
			this->keyState = Hit;
		if ((this->beatCount % 8) == 0)
			this->polyState = Hit;
	}
}


//----------------------
//各状態の更新処理
//----------------------

// フェイト処理完了待ち
void SceneTitle::update_fadeWait(void)
{
	if (FadeScreen::Finished())
	{
		switch (this->com)
		{
		case COM_WAIT:
			if (this->timer > 1.0f)
			{
				this->com = COM_OP;
				// フェイトイン
				FadeScreen::FadeIn(Color::black, 0.7f);
			}
			this->timer += Time::DeltaTime();
			break;

		case COM_OP:
			// BGM再生
			SetVolume(BGM_TITLE, -1800);
			PlayBGM(BGM_TITLE);
			this->beat->reset();
			this->field_poly->SetActive(true);

			// 状態遷移
			SceneTitle::pUpdate = &SceneTitle::update_createPoly;
			break;
		
		case COM_START:
			// シーン遷移
			GameManager::SetScene(new SceneGuide);
			break;

		case COM_EXIT:
			// ゲーム終了
			StopGame();
			break;
		}
	}
}

// 背景ポリゴンの生成
void SceneTitle::update_createPoly(void)
{
	// アニメーションスキップ
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_CI))
		this->field_poly->GetComponent<ParticleSystem>()->emission_rate = TitlePolyMax*100.0f;

	// 処理完了
	if (this->field_poly->IsCompelete())
	{
		// ロゴ表示
		this->logo->SetActive(true);
		this->timer = 0;
		this->logo->GetComponent<RectPolygon>()->SetOpacity(0);

		// 状態遷移
		SceneTitle::pUpdate = &SceneTitle::update_showLogo;
	}
}

// ロゴアニメーション
void SceneTitle::update_showLogo(void)
{
	// アニメーション処理
	const float interval = 0.3f;
	if (this->timer <= interval)
	{
		float progress = this->timer / interval;
		this->logo->transform.scale.x = Lerpf(3, 1, progress);
		this->logo->transform.scale.y = Lerpf(3, 1, progress);
		this->logo->GetComponent<RectPolygon>()->SetOpacity(progress);
	}

	// 処理完了
	else
	{
		this->logo->transform.scale = Vector3(1.0f, 1.0f, 1.0f);

		// PRESS KEY 表示
		this->presskey->SetActive(true);

		// 作者情報表示
		this->info->SetActive(true);

		this->logoState = Wait;
		this->keyState = Wait;
		this->polyState = Wait;

		// 状態遷移
		SceneTitle::pUpdate = &SceneTitle::update_pressWait;
	}

}

// 入力待ち
void SceneTitle::update_pressWait(void)
{
	syncAnimation();

	// メニュー表示
	if (GetKeyboardAnyKeyTrigger() || IsAnyButtonTriggered(0))
	{
		this->timer = 0;
		this->camera->SetState(TitleCamera::State::Normal);
		
		// 状態遷移
		SceneTitle::pUpdate = &SceneTitle::update_showMenu_A;
	}

	update_anime_presskey();
}

// メニューアニメーションA
void SceneTitle::update_showMenu_A(void)
{
	syncAnimation();

	const float interval = 0.1f;

	float progress = this->timer / interval;
	if (this->playBack)
		progress = 1.0f - progress;

	if (this->timer <= interval+0.1f)
	{
		this->presskey->transform.scale = Vector3::Lerp(Vector3(1, 1, 0), Vector3(1.3f, 0.8f, 0), progress);
		this->presskey->GetComponent<RectPolygon2D>()->SetOpacity(Lerpf(1, 0.0f, progress));
	}
	else
	{
		this->presskey->SetActive(this->playBack);

		// メニュー表示
		this->cursor->SetActive(!this->playBack);
		this->start->SetActive(!this->playBack);
		this->exit->SetActive(!this->playBack);

		// 状態遷移
		if (!this->playBack)
			SceneTitle::pUpdate = &SceneTitle::update_showMenu_B;
		else
		{
			this->playBack = false;
			SceneTitle::pUpdate = &SceneTitle::update_pressWait;
		}
		
	}
}

// メニューアニメーションB
void SceneTitle::update_showMenu_B(void)
{
	syncAnimation();

	const float interval = 0.5f;

	float progress = this->timer / interval;
	if (this->timer <= interval + 0.1f)
	{
		this->cursor->GetComponent<RectPolygon>()->SetOpacity(Lerpf(0, 1, progress));
		this->start->GetComponent<RectPolygon>()->SetOpacity(Lerpf(0, 1, progress));
		this->exit->GetComponent<RectPolygon>()->SetOpacity(Lerpf(0, 1, progress));
	}
	else
	{
		this->camera->SetPlay(true);
		this->title_poly->SetMixMode(-1.0f);

		// 状態遷移
		SceneTitle::pUpdate = &SceneTitle::update_menu;
	}
}

// メニュー更新処理
void SceneTitle::update_menu(void)
{
	syncAnimation();

	// メニュー操作
	if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		if (this->cursorPos > 0)
		{
			this->cursor->transform.position.y += 85;
			this->cursorPos--;
		}
	}
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		if (this->cursorPos < 1)
		{
			this->cursor->transform.position.y -= 85;
			this->cursorPos++;
		}
	}
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_CI))
	{
		this->com = (Command)this->cursorPos;

		// 状態遷移
		switch (this->com)
		{
		case COM_START:
			this->logo->SetActive(false);
			this->info->SetActive(false);
			this->cursor->SetActive(false);
			this->start->SetActive(false);
			this->exit->SetActive(false);

			this->title_poly->SetMixMode(1.0f);

			this->camera->SetState(TitleCamera::State::Far);
			this->camera->SetPlay(false);
			
			SceneTitle::pUpdate = &SceneTitle::update_zoomWait;
			break;

		case COM_EXIT:
			FadeScreen::FadeOut(Color::black);
			SceneTitle::pUpdate = &SceneTitle::update_fadeWait;
			break;
		}
	}

	// １つ前に戻る
	if (GetKeyboardTrigger(DIK_BACKSPACE) || IsButtonPressed(0, BUTTON_CR))
	{
		this->timer = 0;
		this->playBack = true;
		this->camera->SetPlay(false);
		this->camera->SetState(TitleCamera::State::Orthogonal);
		this->title_poly->SetMixMode(0.0f);
		this->cursor->transform.position = Vector3(-80, SystemParameters::ResolutionY / 2.f - 415, 0);
		this->cursorPos = 0;
		SceneTitle::pUpdate = &SceneTitle::update_showMenu_A;
	}


}

// ズームアウト待ち処理
void SceneTitle::update_zoomWait(void)
{
	if (this->title_poly->GetMixFactor() >= 1.0f)
	{
		this->field_poly->SetActive(false);
		this->title_poly->SetBehavior2();
		FadeScreen::FadeOut(Color::black, 1.f);
		SceneTitle::pUpdate = &SceneTitle::update_fadeWait;
	}
}


//----------------------
//各ループアニメーション処理
//----------------------

// 背景ポリゴン
void SceneTitle::update_anime_poly(void)
{
	static float timer;
	//switch (this->polyState)
	//{
	//case Hit:
	//	for (int i = 0; i < this->polyCount; i++)
	//	{
	//		this->polyList[i]->prePos = this->polyList[i]->transform.position;
	//		this->polyList[i]->nextColor.setRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 200);
	//	}
	//	this->polyState = Phase1;
	//	timer = 0;
	//	break;
	//case Phase1:
	//	float interval = 2.0f;
	//	if (timer <= interval)
	//	{
	//		float progress = timer / interval;
	//		for (int i = 0; i < this->polyCount; i++)
	//			this->polyList[i]->GetComponent<RectPolygon>()->SetColor(LerpC(this->polyList[i]->GetComponent<RectPolygon>()->GetColor(), this->polyList[i]->nextColor, progress));
	//	}
	//	else
	//	{
	//		this->polyState = Wait;
	//	}
	//	timer += Time::DeltaTime();
	//	break;


	//}

}

// ロゴ
void SceneTitle::update_anime_logo(void)
{
	static float timer;
	switch (this->logoState)
	{
	case Hit:
		this->logoState = Phase1;
		timer = 0;
		break;
	case Phase1:
	{
		float interval = 0.05f;
		if (timer <= interval)
		{
			float porgress = timer / interval;
			this->logo->transform.scale = Vector3(1, 1, 0)*Lerpf(1.0f, 1.13f, porgress);
		}
		else
		{
			this->logoState = Phase2;
			timer = 0;
		}
		timer += Time::DeltaTime();
		break;
	}
	case Phase2:
		this->logo->transform.scale = Vector3(1, 1, 0)*Lerpf(this->logo->transform.scale.x, 1.0f, Time::DeltaTime()*3.0f);
		break;
	}

}

// PRESS KEY
void SceneTitle::update_anime_presskey(void)
{
	static float timer;
	switch (this->keyState)
	{
	case Hit:
		this->keyState = Phase1;
		timer = 0;
		break;
	case Phase1:
	{
		float interval = 0.05f;
		if (timer <= interval)
		{
			float porgress = timer / interval;
			this->presskey->GetComponent<RectPolygon2D>()->SetOpacity(Lerpf(0.95f, 0.5f, porgress));
		}
		else
		{
			this->keyState = Phase2;
			timer = 0;
		}
		timer += Time::DeltaTime();
		break;
	}
	case Phase2:
		this->presskey->GetComponent<RectPolygon2D>()->SetOpacity(Lerpf(this->presskey->GetComponent<RectPolygon2D>()->GetOpacity(), 0.95f, Time::DeltaTime()*3.0f));
		break;
	}

}
