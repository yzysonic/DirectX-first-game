#include "SceneGame.h"
#include "SceneClear.h"
#include "SceneGameOver.h"
#include "FadeScreen.h"
#include "CameraSmooth.h"
#include "CameraShake.h"

/// public�֐�

// ����������
void SceneGame::Init(void)
{
	// ���a�H����
	this->vignetting = new Object;
	this->vignetting->AddComponent<RectPolygon2D>("vignetting", Layer::MASK);

	// �X�R�AUI
	this->scoreUI = new ScoreUI(5, SystemParameters::ResolutionX/2 - 300, SystemParameters::ResolutionY/2 - 30, "number", "game_score");
	this->scoreUI->setOffset(130, 0);

	// �^�C��UI
	const int x_offset = 35;
	this->timeUI[0] = new NumberUI(2, x_offset - SystemParameters::ResolutionX/2, SystemParameters::ResolutionY/2 - 30, "number", "game_time");
	this->timeUI[1] = new NumberUI(2, x_offset + 116 - SystemParameters::ResolutionX/2, SystemParameters::ResolutionY/2 - 30, "number");
	this->timeUI[0]->setOffset(180, 0);

	// �c�@UI 
	this->liveUI = new LiveUI;
	this->liveUI->transform.position = Vector3(x_offset + 3 + Texture::Get("lives")->size.x/2 - SystemParameters::ResolutionX/2, SystemParameters::ResolutionY/2 - 70.f, 0.0f);

	// �v���C���[
	this->player = new Player;
	this->player->SetActive(false);

	// �J�����ݒ�
	this->camera.reset(new Camera());
	this->camera->AddComponent<CameraSmooth>(this->player);
	this->camera->AddComponent<CameraShake>();
	this->camera->SetActive(false);
	this->camera->setBackColor(210, 210, 210, 255);
	Renderer::GetInstance()->setCamera(this->camera.get());

	// �~�j�}�b�v
	this->minimap = new MiniMap(200, 200, 11);
	this->minimap->SetPosition(Vector3(SystemParameters::ResolutionX / 2.0f - 150.0f, -SystemParameters::ResolutionY / 2.0f + 150.0f, 0.0f));
	this->minimap->SetPlayer(this->player);
	this->minimap->zoom = 0.3f;

	// �C�x���g�o�C���h
	this->player->injury += [&]
	{
		// �h��G�t�F�N�g
		this->camera->GetComponent<CameraShake>()->SetActive(true);
		this->minimap->Shake();

		// �c�@�\���̍X�V
		this->liveUI->GetComponent<RectPolygon2D>()->setPattern(this->player->hp - 1);

		// HP���P�ɂȂ������̌x��
		if (this->player->hp == 1)
		{
			this->liveUI->SetLowLive(true);
			PlayBGM(SE_LOW_HP);
			SetVolume(SE_LOW_HP, -1800);
		}
	};

	// �Q�[���Ŏg���ϐ�
	this->score = 0;
	this->timer = GameTime;
	this->polyCount = 0;

	for (int i = 0; i < ENEMY_MAX; i++)
		this->enemy[i] = NULL;

	// �w�i�|���S������
	for (int i = 0; i < GAME_POLY_MAX; i++)
	{
		this->polyList[i] = new PolygonElement;
		this->polyList[i]->transform.position.x = Randomf(-FIELD_RANG_X, FIELD_RANG_X);
		this->polyList[i]->transform.position.y = Randomf(-FIELD_RANG_Y, FIELD_RANG_Y);
		this->polyList[i]->targetOpacity = 0.7f;
		this->polyList[i]->targetScale = Vector3(0.1f, 0.1f, 1.0f);
		int min = (int)(200 * powf((this->polyList[i]->transform.position.z + 100) / 400, 2.0f));
		this->polyList[i]->GetComponent<RectPolygon>()->SetColor(Color(Random(min, 255), Random(min, 255), Random(min, 255), 0));

		this->polyCount++;
	}

	// �t�F�C�g�C������
	FadeScreen::FadeIn(Color::white, 0.7f);
	
	// BGM���Đ�
	SetVolume(BGM_GAME, -1800);
	PlayBGM(BGM_GAME);

	// �Q�[����ԁ��t�F�C�g���������҂�
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

	delete this->minimap;

	for (auto &enemy : this->enemy)
	{
		SafeDelete<Enemy>(enemy);
	}
	delete this->player;
	delete this->vignetting;
	delete this->liveUI;
	delete this->scoreUI;
	delete this->timeUI[0];
	delete this->timeUI[1];
	

	for (int i = 0; i < this->polyCount; i++)
		delete (this->polyList[i]);

	

	Renderer::GetInstance()->setCamera(nullptr);
	Bullet::Clear();
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
	//if (FadeFinished())
	{
		this->player->SetActive(true);
		this->camera->SetActive(true);
		SceneGame::pUpdate = &SceneGame::update_main;
	}
}

void SceneGame::update_main(void)
{
	// �G�̐���
	swapEnemy();

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
			Object::Destroy(enemy);
		}
	}

	// �v���C���[�̈ړ�����
	Vector3 &playerPos = this->player->transform.position;
	if (playerPos.x < -FIELD_RANG_X)
		playerPos.x = -FIELD_RANG_X;
	if (playerPos.x > FIELD_RANG_X)
		playerPos.x = FIELD_RANG_X;
	if (playerPos.y < -FIELD_RANG_Y)
		playerPos.y = -FIELD_RANG_Y;
	if (playerPos.y > FIELD_RANG_Y)
		playerPos.y = FIELD_RANG_Y;


	// �J�E���g�_�E���X�V
	this->timer -= Time::DeltaTime();

	// �J�E���g�_�E���\���X�V
	this->timeUI[0]->setNumber((int)(this->timer*100)%100);
	this->timeUI[1]->setNumber((int)this->timer);
	if (this->timer < 10)
	{
		UCHAR p = (UCHAR)(255 * fabsf(sinf(this->timer*PI / 0.7f)));
		Color c;
		c.setRGBA(255, p, p, 255);
		this->timeUI[0]->SetColor(c);
		this->timeUI[1]->SetColor(c);
	}

	// �V�[���J�ځ��N���A�V�[��
	if (this->timer < 0)
	{
		GameManager::SetScene(new SceneClear);
		return;
	}

	 //�V�[���J�ځ��Q�[���I�[�o�[
	if (this->player->hp == 0)
	{
		GameManager::SetScene(new SceneGameOver);
		return;
	}

}

void SceneGame::swapEnemy(void)
{
	static float timer = 0;

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_E))
		timer = 10;
#endif

	if (timer > 3.0f)
	{
		for (auto &enemy : this->enemy)
		{
			if (enemy == nullptr)
			{
				enemy = new Enemy;

				enemy->target = &this->player->transform;
				enemy->transform.position = Vector3(Randomf(-FIELD_RANG_X + 500, FIELD_RANG_X - 500), Randomf(-FIELD_RANG_Y + 500, FIELD_RANG_Y - 500), 0.0f);
				this->minimap->SetEnemy(enemy);

				break;
			}
		}
		timer = 0;
	}

	timer += Time::DeltaTime();
}
