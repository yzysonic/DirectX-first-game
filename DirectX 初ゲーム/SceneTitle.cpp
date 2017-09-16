#include "SceneTitle.h"
#include "Game.h"
#include "GameManager.h"
#include "Renderer.h"


// ������
void SceneTitle::init(void)
{
	this->polyCount = 0;
	this->beat = new Timer();
	this->beatCount = 0;
	this->timer = 0;
	this->com = COM_WAIT;

	this->vignetting = new Object;
	this->vignetting->setPolygon(Layer::UI_02, TEX_VIGNETTING, RendererType::UI);

	// �A�j���[�V������ԏ�����
	this->logoState =
	this->keyState =
	this->polyState = Wait;



	// �J�����̏�����
	Renderer::GetInstance()->setCamera(NULL);
	Renderer::GetInstance()->getCamera()->backColor = ColorRGBA(243, 242, 238, 255);
	Renderer::GetInstance()->getCamera()->fov = 0.0f;

	// �t�F�C�g�A�E�g
	FadeScreen::Fade(FADE_OUT_BK, 0, 0);

	// �������
	SceneTitle::pUpdate = &SceneTitle::update_fadeWait;
}

// �O���[�o���X�V����
void SceneTitle::update(void)
{
	// �e�A�j���[�V��������
	update_anime_poly();
	update_anime_logo();

	// ���[�J���X�V����
	(this->*pUpdate)();

	this->timer += Time::DeltaTime();
}

// �I������
void SceneTitle::uninit(void)
{
	delete this->cursor;
	delete this->start;
	delete this->exit;
	delete this->logo;
	delete this->presskey;
	delete this->info;
	delete this->vignetting;

	SafeDelete(this->beat);

	for(int i=0;i<this->polyCount;i++)
		delete this->polyList[i];

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
				FadeScreen::Fade(FADE_IN_BK, 0, 0.7f);
			}
			this->timer += Time::DeltaTime();
			break;

		case COM_OP:
			// BGM�Đ�
			SetVolume(BGM_TITLE, -1800);
			PlayBGM(BGM_TITLE);
			this->beat->reset();

			// ��ԑJ��
			SceneTitle::pUpdate = &SceneTitle::update_createPoly;
			break;
		
		case COM_START:
			// �V�[���J��
			GameManager::SetScene(SceneName::GAME);
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
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		for (int i = 0; this->polyCount<TitlePolyMax; i++)
		{
			PolygonElement *poly = new PolygonElement;
			this->polyList[this->polyCount++] = poly;
		}

	}

	// �����A�j���[�V��������
	else if (this->polyCount < TitlePolyMax)
	{
		for (int i = 0; i < 10; i++)
		{
			PolygonElement *poly = new PolygonElement;
			this->polyList[this->polyCount++] = poly;
			if (this->polyCount == TitlePolyMax)
				break;
		}
	}

	// ��������
	else 
	{
		// ���S�\��
		this->logo = new Object;
		this->logo->setPolygon(Layer::UI_00, TEX_TITLE_LOGO, RendererType::UI);
		this->logo->getTransform()->position = Vector3(0, 200 - SCREEN_CENTER_Y, 0);
		this->timer = 0;
		this->logo->getPolygon()->setOpacity(0);

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
		this->logo->getTransform()->scale.x = Lerpf(3, 1, progress);
		this->logo->getTransform()->scale.y = Lerpf(3, 1, progress);
		this->logo->getPolygon()->setOpacity(progress);
	}

	// ��������
	else
	{
		this->logo->getTransform()->scale = Vector3(1.0f, 1.0f, 1.0f);

		// PRESS KEY �\��
		this->presskey = new Object;
		this->presskey->setPolygon(Layer::UI_00, TEX_TITLE_PRESSKEY, RendererType::UI);
		this->presskey->getTransform()->position = Vector3(0, 475 - SCREEN_CENTER_Y, 0);

		// ��ҏ��\��
		this->info = new Object;
		this->info->setPolygon(Layer::UI_02, TEX_TITLE_INFO, RendererType::UI);
		this->info->getTransform()->position.x = SCREEN_CENTER_X - this->info->getPolygon()->pTexture->size.x / 2 - 30;
		this->info->getTransform()->position.y = SCREEN_CENTER_Y - this->info->getPolygon()->pTexture->size.y - 10;
		this->info->getTransform()->position.z = 0;

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
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		this->timer = 0;

		// ��ԑJ��
		SceneTitle::pUpdate = &SceneTitle::update_showMenu_A;
	}

	update_anime_presskey();
}

// ���j���[�A�j���[�V����A
void SceneTitle::update_showMenu_A(void)
{
	syncAnimation();
	Renderer::GetInstance()->getCamera()->fov = Lerpf(Renderer::GetInstance()->getCamera()->fov, 1.0f, Time::DeltaTime() * 10);

	const float interval = 0.1f;

	float progress = this->timer / interval;
	if (this->timer <= interval+0.1f)
	{
		this->presskey->getTransform()->scale = LerpV3(Vector3(1, 1, 0), Vector3(1.3f, 0.8f, 0), progress);
		this->presskey->getPolygon()->setOpacity(Lerpf(1, 0.0f, progress));
	}
	else
	{
		// ���j���[�\��
		this->cursor = new Object;
		this->cursor->setPolygon(Layer::UI_00, TEX_TITLE_CURSOR, RendererType::UI);
		this->cursor->getTransform()->position = Vector3(-80, 415 - SCREEN_CENTER_Y, 0);
		this->cursor->getPolygon()->setOpacity(0);

		this->start = new Object;
		this->start->setPolygon(Layer::UI_00, TEX_TITLE_START, RendererType::UI);
		this->start->getTransform()->position = Vector3(40, 415 - SCREEN_CENTER_Y, 0);
		this->start->getPolygon()->setOpacity(0);

		this->exit = new Object;
		this->exit->setPolygon(Layer::UI_00, TEX_TITLE_EXIT, RendererType::UI);
		this->exit->getTransform()->position = Vector3(40, 500 - SCREEN_CENTER_Y, 0);
		this->exit->getPolygon()->setOpacity(0);

		this->cursorPos = 0;

		// ��ԑJ��
		SceneTitle::pUpdate = &SceneTitle::update_showMenu_B;
	}
}

// ���j���[�A�j���[�V����B
void SceneTitle::update_showMenu_B(void)
{
	syncAnimation();
	Renderer::GetInstance()->getCamera()->fov = Lerpf(Renderer::GetInstance()->getCamera()->fov, 1.0f, Time::DeltaTime() * 10);


	const float interval = 0.5f;

	float progress = this->timer / interval;
	if (this->timer <= interval + 0.1f)
	{
		this->cursor->getPolygon()->setOpacity(Lerpf(0, 1, progress));
		this->start->getPolygon()->setOpacity(Lerpf(0, 1, progress));
		this->exit->getPolygon()->setOpacity(Lerpf(0, 1, progress));
	}
	else
	{
		// ��ԑJ��
		SceneTitle::pUpdate = &SceneTitle::update_menu;
	}
}

// ���j���[�X�V����
void SceneTitle::update_menu(void)
{
	syncAnimation();

	// ���j���[����
	if (GetKeyboardTrigger(DIK_UP))
	{
		if (this->cursorPos > 0)
		{
			this->cursor->getTransform()->position.y -= 85;
			this->cursorPos--;
		}
	}
	if (GetKeyboardTrigger(DIK_DOWN))
	{
		if (this->cursorPos < 1)
		{
			this->cursor->getTransform()->position.y += 85;
			this->cursorPos++;
		}
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		this->com = (Command)this->cursorPos;

		switch (this->com)
		{
		case COM_START:
			FadeScreen::Fade(FADE_OUT_WH, 0);
			break;
		case COM_EXIT:
			FadeScreen::Fade(FADE_OUT_BK, 0);
			break;
		}

		// ��ԑJ��
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
	switch (this->polyState)
	{
	case Hit:
		for (int i = 0; i < this->polyCount; i++)
		{
			this->polyList[i]->prePos = this->polyList[i]->getTransform()->position;
			this->polyList[i]->nextColor = ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 200);
		}
		this->polyState = Phase1;
		timer = 0;
		break;
	case Phase1:
	{
		float interval = 2.0f;
		if (timer <= interval)
		{
			float progress = timer / interval;
			for (int i = 0; i < this->polyCount; i++)
			{
				//Vector3 dir;
				//D3DXVec3Normalize(&dir, &this->polyList[i]->prePos);
				//this->polyList[i]->base->transform->position += dir*10.0f*Time::DeltaTime() / interval;

				//this->polyList[i]->base->transform->scale = Vector3(1, 1, 0)*Lerpf(0.4f, 0.6f, progress);

				this->polyList[i]->getPolygon()->setColor(LerpC(this->polyList[i]->getPolygon()->getColor(), this->polyList[i]->nextColor, progress));
			}
		}
		else
		{
			this->polyState = Wait;
			//this->polyState = Phase2;
		}
		timer += Time::DeltaTime();
		break;
	}
	case Phase2:

		for (int i = 0; i < this->polyCount; i++)
		{
			//this->polyList[i]->base->transform->position = LerpV3(this->polyList[i]->base->transform->position, this->polyList[i]->prePos, Time::DeltaTime()*10.0f);

			this->polyList[i]->getTransform()->scale = Vector3(1, 1, 0)*Lerpf(this->polyList[i]->getTransform()->scale.x, 0.4f, Time::DeltaTime()*3.0f);
		}


		break;
	}

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
			this->logo->getTransform()->scale = Vector3(1, 1, 0)*Lerpf(1.0f, 1.13f, porgress);
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
		this->logo->getTransform()->scale = Vector3(1, 1, 0)*Lerpf(this->logo->getTransform()->scale.x, 1.0f, Time::DeltaTime()*3.0f);
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
			this->presskey->getPolygon()->setOpacity(Lerpf(0.95f, 0.5f, porgress));
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
		this->presskey->getPolygon()->setOpacity(Lerpf(this->presskey->getPolygon()->getOpacity(), 0.95f, Time::DeltaTime()*3.0f));
		break;
	}

}
