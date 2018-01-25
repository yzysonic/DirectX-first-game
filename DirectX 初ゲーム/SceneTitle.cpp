#include "SceneTitle.h"
#include "Core\Game.h"
#include "SceneGuide.h"
#include "FadeScreen.h"
#include "CameraFov.h"


// 初期化
void SceneTitle::Init(void)
{
	this->polyCount = 0;
	this->beat = new Timer();
	this->beatCount = 0;
	this->timer = 0;
	this->com = COM_WAIT;
	this->playBack = false;
	this->cursorPos = 0;

	this->vignetting = new Object;
	this->vignetting->AddComponent<RectPolygon2D>("vignetting", Layer::UI_02);

	this->logo = new Object;
	this->logo->AddComponent<RectPolygon2D>("title_logo", Layer::UI_00);
	this->logo->transform.position = Vector3(0, SystemParameters::ResolutionY / 2.f - 200, 0);
	this->logo->SetActive(false);

	this->presskey = new Object;
	this->presskey->AddComponent<RectPolygon2D>("title_presskey", Layer::UI_00);
	this->presskey->transform.position = Vector3(0, SystemParameters::ResolutionY / 2.f - 475, 0);
	this->presskey->SetActive(false);

	this->info = new Object;
	this->info->AddComponent<RectPolygon2D>("title_info", Layer::UI_02);
	this->info->transform.position.x = SystemParameters::ResolutionX / 2 - this->info->GetComponent<RectPolygon2D>()->pTexture->size.x / 2 - 30;
	this->info->transform.position.y = -SystemParameters::ResolutionY / 2 + this->info->GetComponent<RectPolygon2D>()->pTexture->size.y + 10;
	this->info->transform.position.z = 0;
	this->info->SetActive(false);

	this->cursor = new Object;
	this->cursor->AddComponent<RectPolygon2D>("title_cursor", Layer::UI_00);
	this->cursor->GetComponent<RectPolygon2D>()->SetOpacity(0);
	this->cursor->transform.position = Vector3(-80, SystemParameters::ResolutionY / 2.f - 415, 0);
	this->cursor->SetActive(false);

	this->start = new Object;
	this->start->AddComponent<RectPolygon2D>("title_start", Layer::UI_00);
	this->start->GetComponent<RectPolygon2D>()->SetOpacity(0);
	this->start->transform.position = Vector3(40, SystemParameters::ResolutionY / 2.f - 415, 0);
	this->start->SetActive(false);
	
	this->exit = new Object;
	this->exit->AddComponent<RectPolygon2D>("title_exit", Layer::UI_00);
	this->exit->GetComponent<RectPolygon2D>()->SetOpacity(0);
	this->exit->transform.position = Vector3(40, SystemParameters::ResolutionY / 2.f - 500, 0);
	this->exit->SetActive(false);

	// アニメーション状態初期化
	this->logoState =
	this->keyState =
	this->polyState = Wait;

	// カメラの初期化
	this->camera.reset(new Camera);
	this->camera->fov = Deg2Rad(1.0f);
	this->camera->transform.position.z = -0.5f*SystemParameters::ResolutionY / tanf(0.5f*Deg2Rad(1.0f));
	this->camera->setBackColor(243, 242, 238, 255);
	this->camera->AddComponent<CameraFov>()->SetActive(false);
	Renderer::GetInstance()->setCamera(this->camera.get());

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
	SafeDelete(this->cursor);
	SafeDelete(this->start);
	SafeDelete(this->exit);
	SafeDelete(this->logo);
	SafeDelete(this->presskey);
	SafeDelete(this->info);
	SafeDelete(this->vignetting);
	SafeDelete(this->beat);

	for(int i=0;i<this->polyCount;i++)
		SafeDelete(this->polyList[i]);

	Renderer::GetInstance()->setCamera(nullptr);

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
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		while (this->polyCount<TitlePolyMax)
		{
			PolygonElement *poly = new PolygonElement;
			poly->targetScale = Vector3(0.5f, 0.5f, 1.0f);
			this->polyList[this->polyCount++] = poly;
		}

	}

	// 生成アニメーション処理
	else if (this->polyCount < TitlePolyMax)
	{
		for (int i = 0; i < 10; i++)
		{
			PolygonElement *poly = new PolygonElement;
			poly->targetScale = Vector3(0.5f, 0.5f, 1.0f);
			this->polyList[this->polyCount++] = poly;
			if (this->polyCount == TitlePolyMax)
				break;
		}
	}

	// 処理完了
	else 
	{
		// ロゴ表示
		this->logo->SetActive(true);
		this->timer = 0;
		this->logo->GetComponent<RectPolygon2D>()->SetOpacity(0);

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
		this->logo->GetComponent<RectPolygon2D>()->SetOpacity(progress);
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
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_CI))
	{
		this->timer = 0;

		// 状態遷移
		SceneTitle::pUpdate = &SceneTitle::update_showMenu_A;
	}

	update_anime_presskey();
}

// メニューアニメーションA
void SceneTitle::update_showMenu_A(void)
{
	syncAnimation();
	if (this->playBack)
		this->camera->GetComponent<CameraFov>()->SetFov(Lerpf(this->camera->fov, Deg2Rad(1.0f), Time::DeltaTime() * 10));
	else
		this->camera->GetComponent<CameraFov>()->SetFov(Lerpf(this->camera->fov, Deg2Rad(120.0f), Time::DeltaTime() * 10));

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
	this->camera->GetComponent<CameraFov>()->SetFov(Lerpf(this->camera->fov, Deg2Rad(120.0f), Time::DeltaTime() * 10));


	const float interval = 0.5f;

	float progress = this->timer / interval;
	if (this->timer <= interval + 0.1f)
	{
		this->cursor->GetComponent<RectPolygon2D>()->SetOpacity(Lerpf(0, 1, progress));
		this->start->GetComponent<RectPolygon2D>()->SetOpacity(Lerpf(0, 1, progress));
		this->exit->GetComponent<RectPolygon2D>()->SetOpacity(Lerpf(0, 1, progress));
	}
	else
	{
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

		switch (this->com)
		{
		case COM_START:
			FadeScreen::FadeOut(Color::white);
			break;
		case COM_EXIT:
			FadeScreen::FadeOut(Color::black);
			break;
		}

		// 状態遷移
		SceneTitle::pUpdate = &SceneTitle::update_fadeWait;
	}

	// １つ前に戻る
	if (GetKeyboardTrigger(DIK_BACKSPACE) || IsButtonPressed(0, BUTTON_CR))
	{
		this->timer = 0;
		this->playBack = true;
		SceneTitle::pUpdate = &SceneTitle::update_showMenu_A;
	}


}


//----------------------
//各ループアニメーション処理
//----------------------

// 背景ポリゴン
void SceneTitle::update_anime_poly(void)
{
	static float timer;
	switch (this->polyState)
	{
	case Hit:
		for (int i = 0; i < this->polyCount; i++)
		{
			this->polyList[i]->prePos = this->polyList[i]->transform.position;
			this->polyList[i]->nextColor.setRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 200);
		}
		this->polyState = Phase1;
		timer = 0;
		break;
	case Phase1:
		float interval = 2.0f;
		if (timer <= interval)
		{
			float progress = timer / interval;
			for (int i = 0; i < this->polyCount; i++)
				this->polyList[i]->GetComponent<RectPolygon>()->SetColor(LerpC(this->polyList[i]->GetComponent<RectPolygon>()->GetColor(), this->polyList[i]->nextColor, progress));
		}
		else
		{
			this->polyState = Wait;
		}
		timer += Time::DeltaTime();
		break;


	}

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
