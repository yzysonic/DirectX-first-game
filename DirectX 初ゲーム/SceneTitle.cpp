#include "SceneTitle.h"
#include "Core\Game.h"
#include "SceneGuide.h"
#include "FadeScreen.h"


// ������
void SceneTitle::init(void)
{
	this->polyCount = 0;
	this->beat = new Timer();
	this->beatCount = 0;
	this->timer = 0;
	this->com = COM_WAIT;
	this->playBack = false;
	this->cursorPos = 0;


	this->vignetting = new Object2D;
	this->vignetting->setPolygon(Layer::UI_02, Texture::Get("vignetting"), RendererType::UI);
	this->vignetting->setActive(false);

	this->logo = new Object2D;
	this->logo->setPolygon(Layer::UI_00, Texture::Get("title_logo"), RendererType::UI);
	this->logo->getTransform()->position = Vector3(0, SystemParameters::ResolutionY / 2.f - 200, 0);
	this->logo->setActive(false);

	this->presskey = new Object2D;
	this->presskey->setPolygon(Layer::UI_00, Texture::Get("title_presskey"), RendererType::UI);
	this->presskey->getTransform()->position = Vector3(0, SystemParameters::ResolutionY / 2.f - 475, 0);
	this->presskey->setActive(false);

	this->info = new Object2D;
	this->info->setPolygon(Layer::UI_02, Texture::Get("title_info"), RendererType::UI);
	this->info->getTransform()->position.x = SystemParameters::ResolutionX / 2 - this->info->getPolygon()->pTexture->size.x / 2 - 30;
	this->info->getTransform()->position.y = -SystemParameters::ResolutionY / 2 + this->info->getPolygon()->pTexture->size.y + 10;
	this->info->getTransform()->position.z = 0;
	this->info->setActive(false);

	this->cursor = new Object2D;
	this->cursor->setPolygon(Layer::UI_00, Texture::Get("title_cursor"), RendererType::UI);
	this->cursor->getTransform()->position = Vector3(-80, SystemParameters::ResolutionY / 2.f - 415, 0);
	this->cursor->getPolygon()->setOpacity(0);
	this->cursor->setActive(false);

	this->start = new Object2D;
	this->start->setPolygon(Layer::UI_00, Texture::Get("title_start"), RendererType::UI);
	this->start->getTransform()->position = Vector3(40, SystemParameters::ResolutionY / 2.f - 415, 0);
	this->start->getPolygon()->setOpacity(0);
	this->start->setActive(false);
	
	this->exit = new Object2D;
	this->exit->setPolygon(Layer::UI_00, Texture::Get("title_exit"), RendererType::UI);
	this->exit->getTransform()->position = Vector3(40, SystemParameters::ResolutionY / 2.f - 500, 0);
	this->exit->getPolygon()->setOpacity(0);
	this->exit->setActive(false);

	// �A�j���[�V������ԏ�����
	this->logoState =
	this->keyState =
	this->polyState = Wait;

	// �J�����̏�����
	Renderer::GetInstance()->setCamera(NULL);
	Renderer::GetInstance()->getCamera()->setBackColor(243, 242, 238, 255);
	Renderer::GetInstance()->getCamera()->fov = 0.0f;

	// �t�F�C�g�A�E�g
	FadeScreen::FadeOut(Color::black, 0);

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
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		while (this->polyCount<TitlePolyMax)
		{
			PolygonElement *poly = new PolygonElement;
			poly->targetScale = Vector3(0.5f, 0.5f, 1.0f);
			this->polyList[this->polyCount++] = poly;
		}

	}

	// �����A�j���[�V��������
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

	// ��������
	else 
	{
		// ���S�\��
		this->logo->setActive(true);
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
		this->presskey->setActive(true);

		// ��ҏ��\��
		this->info->setActive(true);

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
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_CI))
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
	if(this->playBack)
		Renderer::GetInstance()->getCamera()->fov = Lerpf(Renderer::GetInstance()->getCamera()->fov, 0.0f, Time::DeltaTime() * 10);
	else
		Renderer::GetInstance()->getCamera()->fov = Lerpf(Renderer::GetInstance()->getCamera()->fov, 1.0f, Time::DeltaTime() * 10);

	const float interval = 0.1f;

	float progress = this->timer / interval;
	if (this->playBack)
		progress = 1.0f - progress;

	if (this->timer <= interval+0.1f)
	{
		this->presskey->getTransform()->scale = Vector3::Lerp(Vector3(1, 1, 0), Vector3(1.3f, 0.8f, 0), progress);
		this->presskey->getPolygon()->setOpacity(Lerpf(1, 0.0f, progress));
	}
	else
	{
		// ���j���[�\��
		this->cursor->setActive(!this->playBack);
		this->start->setActive(!this->playBack);
		this->exit->setActive(!this->playBack);

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
	if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		if (this->cursorPos > 0)
		{
			this->cursor->getTransform()->position.y += 85;
			this->cursorPos--;
		}
	}
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		if (this->cursorPos < 1)
		{
			this->cursor->getTransform()->position.y -= 85;
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

		// ��ԑJ��
		SceneTitle::pUpdate = &SceneTitle::update_fadeWait;
	}

	// �P�O�ɖ߂�
	if (GetKeyboardTrigger(DIK_BACKSPACE) || IsButtonPressed(0, BUTTON_CR))
	{
		this->timer = 0;
		this->playBack = true;
		SceneTitle::pUpdate = &SceneTitle::update_showMenu_A;
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
			this->polyList[i]->nextColor.setRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 200);
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
