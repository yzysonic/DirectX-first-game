#include "SceneTitle.h"
#include "Core\Game.h"
#include "SceneGuide.h"
#include "FadeScreen.h"
#include "CameraFov.h"


// ������
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

	// �A�j���[�V������ԏ�����
	this->logoState =
	this->keyState =
	this->polyState = Wait;

	// �J�����̏�����
	this->camera = (new TitleCamera);

	// �w�i�|���S���̏�����
	this->field_poly = new FieldPolygon(TitlePolyMax/6);
	this->title_poly = new TitlePoly(TitlePolyMax*5/6);
	this->field_poly->SetActive(false);

	// �t�F�C�g�A�E�g
	FadeScreen::FadeOut(Color::black, 0);

	// �������
	SceneTitle::pUpdate = &SceneTitle::update_fadeWait;
}

// �O���[�o���X�V����
void SceneTitle::Update(void)
{
	// �e�A�j���[�V��������
	update_anime_poly();
	update_anime_logo();

	// ���[�J���X�V����
	(this->*pUpdate)();

	this->timer += Time::DeltaTime();
}

// �I������
void SceneTitle::Uninit(void)
{
	SafeDelete(this->beat);
	StopSound(BGM_TITLE);
}


// �A�j���[�V������BGM�̓�������
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
//�e��Ԃ̍X�V����
//----------------------

// �t�F�C�g���������҂�
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
				// �t�F�C�g�C��
				FadeScreen::FadeIn(Color::black, 0.7f);
			}
			this->timer += Time::DeltaTime();
			break;

		case COM_OP:
			// BGM�Đ�
			SetVolume(BGM_TITLE, -1800);
			PlayBGM(BGM_TITLE);
			this->beat->reset();
			this->field_poly->SetActive(true);

			// ��ԑJ��
			SceneTitle::pUpdate = &SceneTitle::update_createPoly;
			break;
		
		case COM_START:
			// �V�[���J��
			GameManager::SetScene(new SceneGuide);
			break;

		case COM_EXIT:
			// �Q�[���I��
			StopGame();
			break;
		}
	}
}

// �w�i�|���S���̐���
void SceneTitle::update_createPoly(void)
{
	// �A�j���[�V�����X�L�b�v
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_CI))
		this->field_poly->GetComponent<ParticleSystem>()->emission_rate = TitlePolyMax*100.0f;

	// ��������
	if (this->field_poly->IsCompelete())
	{
		// ���S�\��
		this->logo->SetActive(true);
		this->timer = 0;
		this->logo->GetComponent<RectPolygon>()->SetOpacity(0);

		// ��ԑJ��
		SceneTitle::pUpdate = &SceneTitle::update_showLogo;
	}
}

// ���S�A�j���[�V����
void SceneTitle::update_showLogo(void)
{
	// �A�j���[�V��������
	const float interval = 0.3f;
	if (this->timer <= interval)
	{
		float progress = this->timer / interval;
		this->logo->transform.scale.x = Lerpf(3, 1, progress);
		this->logo->transform.scale.y = Lerpf(3, 1, progress);
		this->logo->GetComponent<RectPolygon>()->SetOpacity(progress);
	}

	// ��������
	else
	{
		this->logo->transform.scale = Vector3(1.0f, 1.0f, 1.0f);

		// PRESS KEY �\��
		this->presskey->SetActive(true);

		// ��ҏ��\��
		this->info->SetActive(true);

		this->logoState = Wait;
		this->keyState = Wait;
		this->polyState = Wait;

		// ��ԑJ��
		SceneTitle::pUpdate = &SceneTitle::update_pressWait;
	}

}

// ���͑҂�
void SceneTitle::update_pressWait(void)
{
	syncAnimation();

	// ���j���[�\��
	if (GetKeyboardAnyKeyTrigger() || IsAnyButtonTriggered(0))
	{
		this->timer = 0;
		this->camera->SetState(TitleCamera::State::Normal);
		
		// ��ԑJ��
		SceneTitle::pUpdate = &SceneTitle::update_showMenu_A;
	}

	update_anime_presskey();
}

// ���j���[�A�j���[�V����A
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

		// ���j���[�\��
		this->cursor->SetActive(!this->playBack);
		this->start->SetActive(!this->playBack);
		this->exit->SetActive(!this->playBack);

		// ��ԑJ��
		if (!this->playBack)
			SceneTitle::pUpdate = &SceneTitle::update_showMenu_B;
		else
		{
			this->playBack = false;
			SceneTitle::pUpdate = &SceneTitle::update_pressWait;
		}
		
	}
}

// ���j���[�A�j���[�V����B
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

		// ��ԑJ��
		SceneTitle::pUpdate = &SceneTitle::update_menu;
	}
}

// ���j���[�X�V����
void SceneTitle::update_menu(void)
{
	syncAnimation();

	// ���j���[����
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

		// ��ԑJ��
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

	// �P�O�ɖ߂�
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

// �Y�[���A�E�g�҂�����
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
//�e���[�v�A�j���[�V��������
//----------------------

// �w�i�|���S��
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

// ���S
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
