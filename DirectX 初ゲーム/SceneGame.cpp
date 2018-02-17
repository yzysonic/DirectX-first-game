#include "SceneGame.h"
#include "SceneClear.h"
#include "SceneTitle.h"
#include "SceneGameOver.h"
#include "ScenePause.h"
#include "FadeScreen.h"
#include "CameraSmooth.h"
#include "CameraShake.h"
#include "PostEffect.h"

/// public�֐�

// ����������
void SceneGame::Init(void)
{
	// �|�X�g�G�t�F�N�g
	GameManager::Var<PostEffect*>("post_effect")->SetActive(true);

	// ���a�H����
	this->vignetting = (new Object);
	this->vignetting->AddComponent<RectPolygon2D>("vignetting", Layer::MASK);

	// �X�R�AUI
	this->scoreUI = (new ScoreUI(5, SystemParameters::ResolutionX/2 - 300, SystemParameters::ResolutionY/2 - 30, "number", "game_score"));
	this->scoreUI->setOffset(130, 0);

	// �^�C��UI
	const int x_offset = 35;
	this->timeUI[0] = (new NumberUI(2, x_offset - SystemParameters::ResolutionX/2, SystemParameters::ResolutionY/2 - 30, "number", "game_time"));
	this->timeUI[1] = (new NumberUI(2, x_offset + 116 - SystemParameters::ResolutionX/2, SystemParameters::ResolutionY/2 - 30, "number"));
	this->timeUI[0]->setOffset(180, 0);

	// ���E
	this->boundary = new FieldBoundary(2 * FIELD_RANG_X + 150, 2 * FIELD_RANG_Y + 150);

	// �c�@UI 
	this->liveUI = (new LiveUI);
	this->liveUI->transform.position = Vector3(x_offset + 3 + Texture::Get("lives")->size.x/2 - SystemParameters::ResolutionX/2, SystemParameters::ResolutionY/2 - 70.f, 0.0f);

	// ���Ԑ؂�
	this->timesup = new Object;
	this->timesup->transform.position.y += 100.0f;
	this->timesup->AddComponent<RectPolygon2D>("timesup", Layer::UI_00)->SetOpacity(0.0f);
	this->timesup->GetComponent<RectPolygon2D>()->SetActive(false);

	// �v���C���[
	this->player = (new Player);

	// �J�����ݒ�
	this->camera = new Camera(GameManager::Var<PostEffect*>("post_effect")->GetInputRT());
	this->camera->AddComponent<CameraSmooth>(this->player);
	this->camera->AddComponent<CameraShake>();
	this->camera->transform.position.z = -1500.0f;
	this->camera->setBackColor(210, 210, 210, 255);
	Renderer::GetInstance()->setCamera(this->camera);

	// �~�j�}�b�v
	this->minimap = (new MiniMap(200, 200, 11));
	this->minimap->SetPosition(Vector3(SystemParameters::ResolutionX / 2.0f - 150.0f, -SystemParameters::ResolutionY / 2.0f + 150.0f, 0.0f));
	this->minimap->SetPlayer(this->player);
	this->minimap->zoom = 0.3f;

	// �C�x���g�o�C���h
	//�_���[�W�C�x���g
	this->player->event_injury += [&]
	{
		// �h��G�t�F�N�g
		this->camera->GetComponent<CameraShake>()->Shake();
		this->minimap->Shake();

		// �c�@�\���̍X�V
		this->liveUI->GetComponent<RectPolygon2D>()->setPattern(this->player->hp - 1);

		// HP���P�ɂȂ������̌x��
		if (this->player->hp == 1)
		{
			this->liveUI->SetLowLive(true);
			PlayBGM(SE_LOW_HP);
			SetVolume(SE_LOW_HP, -1800);
			GameManager::Var<PostEffect*>("post_effect")->SetCA(5.0f);
		}
	};
	// ���[�u�C�x���g
	this->player->event_move += [&]
	{
		player_limit();
	};

	// �Q�[���Ŏg���ϐ�
	this->score = 0;
	this->timer = GameTime;
	this->enemy_timer.Reset(3.0f);
	this->polyCount = 0;

	for (int i = 0; i < ENEMY_MAX; i++)
		this->enemy[i] = NULL;

	// �w�i�|���S��������
	this->field_poly = new FieldPolygon(5000, SystemParameters::ResolutionX+1500.0f, SystemParameters::ResolutionY+1500.0f, 10.0f, 100000.0f, 0.5);

	// �t�F�C�g�C������
	FadeScreen::FadeIn(Color::white, 1.0f);
	
	// BGM���Đ�
	SetVolume(BGM_GAME, -1800);
	PlayBGM(BGM_GAME);

	// �Q�[����ԁ��t�F�C�g���������҂�
	this->next_state = NextState::Start;
	SceneGame::pUpdate = &SceneGame::update_fadeWait;
}

// �O���[�o���X�V����
void SceneGame::Update(void)
{
	(this->*pUpdate)();
}

// �I������
void SceneGame::Uninit(void)
{
	GameManager::Var<int>("score") = this->score;

	StopSound(BGM_GAME);

	Renderer::GetInstance()->setCamera(nullptr);
	Bullet::Clear();

	GameManager::Var<PostEffect*>("post_effect")->SetActive(false);
}

void SceneGame::OnPause(void)
{
	RenderSpace::Get("default")->RemoveCamera(this->camera);
	GameManager::Var<PostEffect*>("post_effect")->Pause();
	SetVolume(BGM_GAME, -2500);
}

void SceneGame::OnResume(void)
{
	Renderer::GetInstance()->setCamera(this->camera);
	GameManager::Var<PostEffect*>("post_effect")->Resume();
	SetVolume(BGM_GAME, -1800);
}

// �X�R�A�̎擾
int SceneGame::getGameScore(void)
{
	return this->score;
}

void SceneGame::addGameScore(int score)
{
	this->score += score;
}


/// private�֐�

// �t�@�C�g���������҂�
void SceneGame::update_fadeWait(void)
{
	if (!FadeScreen::Finished())
		return;

	switch (this->next_state)
	{
	case NextState::Start:
		SceneGame::pUpdate = &SceneGame::update_main;
		return;

	case NextState::GameOver:
		GameManager::SetScene(new SceneGameOver);
		return;

	case NextState::GameClear:
		GameManager::SetScene(new SceneClear);
		return;
	}
}

void SceneGame::update_main(void)
{
	// �G�̐���
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_E))
		swapEnemy();
#endif
	if (this->enemy_timer.TimeUp())
	{
		swapEnemy();
		this->enemy_timer.Reset();
	}
	this->enemy_timer++;
	
	// �G�̎��S����
	for (auto &enemy : this->enemy)
	{
		if (enemy == nullptr)
			continue;

		if (enemy->hp == 0)
		{
			// �X�R�A�X�V
			this->addGameScore(300);
			this->scoreUI->SetScore(this->score);
			this->minimap->RemoveEnemy(enemy);
			enemy->SetDeath();
			enemy = nullptr;
		}
	}

	player_limit();

	// �J�E���g�_�E���X�V
	this->timer = fmaxf(this->timer-Time::DeltaTime(), 0.0f);

	// �J�E���g�_�E���\���X�V
	this->timeUI[0]->setNumber((int)(this->timer*100)%100);
	this->timeUI[1]->setNumber((int)this->timer);
	if (this->timer < 10)
	{
		UCHAR p = (UCHAR)(255 * fabsf(sinf(this->timer*PI / 0.7f)));
		Color c(255, p, p, 255);
		this->timeUI[0]->SetColor(c);
		this->timeUI[1]->SetColor(c);
	}

	// ���Ԑ؂�
	if (this->timer <= 0.0f)
	{
		this->player->SetActive(false);
		this->player->GetComponent<RectPolygon>()->SetActive(true);
		this->timer = 0.0f;
		this->timesup->GetComponent<RectPolygon2D>()->SetActive(true);
		SceneGame::pUpdate = &SceneGame::update_timeup;
		return;
	}

	 // ���S
	if (this->player->hp == 0)
	{
		this->player->SetDeath();
		this->liveUI->SetActive(false);
		this->camera->GetComponent<CameraShake>()->Shake(100);
		GameManager::Var<PostEffect*>("post_effect")->SetCA(0.0f);
		SceneGame::pUpdate = &SceneGame::update_death;
		return;
	}

	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_OPTIONS) || IsButtonPressed(0, BUTTON_PAD))
	{
		GameManager::PushScene(new ScenePause);
	}
}

void SceneGame::update_death(void)
{
	if (this->player->state == Player::State::Vanish)
	{
		this->next_state = NextState::GameOver;
		SceneGame::pUpdate = &SceneGame::update_fadeWait;
		FadeScreen::FadeOut(Color::black);
	}
}

void SceneGame::update_timeup(void)
{
	if (this->timer >= 3.0f)
	{
		this->next_state = NextState::GameClear;
		SceneGame::pUpdate = &SceneGame::update_fadeWait;
		FadeScreen::FadeOut(Color::white);
		return;
	}
	else if (this->timer <= 0.3f)
		this->timesup->GetComponent<RectPolygon2D>()->SetOpacity(Lerpf(0.0f, 1.0f, this->timer / 0.3f));

	this->timer += Time::DeltaTime();
}

void SceneGame::swapEnemy(void)
{
	for (auto &enemy : this->enemy)
	{
		if (enemy == nullptr)
		{
			enemy = (new Enemy);

			enemy->target = &this->player->transform;
			do {
				enemy->transform.position = Vector3(Randomf(-FIELD_RANG_X, FIELD_RANG_X), Randomf(-FIELD_RANG_Y, FIELD_RANG_Y), 0.0f);
			} while ((enemy->transform.position - this->player->transform.position).length() < 200.0f);
			this->minimap->SetEnemy(enemy);

			break;
		}
	}
}

void SceneGame::player_limit(void)
{
	// �v���C���[�̈ړ�����
	Vector3 &playerPos = this->player->transform.position;
	if (playerPos.x < -FIELD_RANG_X)
	{
		playerPos.x = Lerpf(playerPos.x, -FIELD_RANG_X, 0.3f);
		this->boundary->Touch();
	}
	if (playerPos.x > FIELD_RANG_X)
	{
		playerPos.x = Lerpf(playerPos.x, FIELD_RANG_X, 0.3f);
		this->boundary->Touch();
	}
	if (playerPos.y < -FIELD_RANG_Y)
	{
		playerPos.y = Lerpf(playerPos.y, -FIELD_RANG_Y, 0.3f);
		this->boundary->Touch();
	}
	if (playerPos.y > FIELD_RANG_Y)
	{
		playerPos.y = Lerpf(playerPos.y, FIELD_RANG_Y, 0.3f);
		this->boundary->Touch();
	}

}
