#include "SceneGame.h"
#include "GameManager.h"
#include "Core.h"
#include "NumberUI.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PolygonElement.h"


typedef struct _SceneGame
{
	Scene base;

	Object* vignetting = NULL;
	Object* liveUI = NULL;
	NumberUI* scoreUI = NULL;
	NumberUI* timeUI[2] = {};
	Camera* camera = NULL;
	PolygonElement *polyList[GAME_POLY_MAX] = {};
	Player* player = NULL;
	Enemy* enemy[ENEMY_MAX] = {};
	
	int score;
	float timer;
	int polyCount;

	void(*update)(void);
}SceneGame;

// �O���[�o���ϐ��錾
SceneGame g_SceneGame;
SceneGame *thiz = &g_SceneGame;

// �v���g�^�C�v�錾
void update_game_fadeWait(void);
void update_game_main(void);
void swapEnemy(void);


/// public�֐�

// �C���X�^���X�̎擾
Scene * GetSceneGame(void)
{
	return (Scene*)&g_SceneGame;
}

// ����������
void initSceneGame(void)
{
	// ���a�H����
	thiz->vignetting = newObject(&thiz->vignetting);
	thiz->vignetting->polygon = newPolygon(thiz->vignetting, LAYER_MASK, TEX_VIGNETTING, REND_UI);

	// �X�R�AUI
	thiz->scoreUI = newNumberUI(5, SCREEN_CENTER_X - 300, 30 - SCREEN_CENTER_Y, TEX_NUMBER, TEX_GAME_SCORE);
	NumberUI_SetOffset(thiz->scoreUI, 130, 0);

	// �^�C��UI
	const int x_offset = 35;
	thiz->timeUI[0] = newNumberUI(2, x_offset - SCREEN_CENTER_X, 30 - SCREEN_CENTER_Y, TEX_NUMBER, TEX_GAME_TIME);
	thiz->timeUI[1] = newNumberUI(2, x_offset + 116 - SCREEN_CENTER_X, 30 - SCREEN_CENTER_Y, TEX_NUMBER);
	NumberUI_SetOffset(thiz->timeUI[0], 180, 0);

	// �c�@UI 
	thiz->liveUI = newObject(&thiz->liveUI);
	thiz->liveUI->polygon = newPolygon(thiz->liveUI, LAYER_UI_00, TEX_LIFES, REND_UI);
	thiz->liveUI->transform->position = Vector3(x_offset + 3 + GetTexture(TEX_LIFES)->size.x/2 - SCREEN_CENTER_X, 70 - SCREEN_CENTER_Y, 0.0f);

	// �v���C���[
	thiz->player = NewSubObj(Player);
	Object_SetActive(thiz->player->base, false);

	// �J����
	thiz->camera = newCamera(thiz->player->base->transform, Vector3(0, -50, -5));
	Object_SetActive(thiz->camera->base, false);

	// �����_���[�ݒ�
	Renderer_SetCamera(thiz->camera->base->transform);
	Renderer_SetBackColor(ColorRGBA(210, 210, 210, 255));
	Renderer_SetFov(1);

	// �Q�[���Ŏg���ϐ�
	thiz->score = 0;
	thiz->timer = 60.0f;
	thiz->polyCount = 0;

	for (int i = 0; i < ENEMY_MAX; i++)
		thiz->enemy[i] = NULL;

	// �w�i�|���S������
	for (int i = 0; i < GAME_POLY_MAX; i++)
	{
		thiz->polyList[i] = NewSubObj(PolygonElement);
		thiz->polyList[i]->base->transform->position.x = Randomf(-FIELD_RANG_X, FIELD_RANG_X);
		thiz->polyList[i]->base->transform->position.y = Randomf(-FIELD_RANG_Y, FIELD_RANG_Y);
		thiz->polyList[i]->base->transform->position.z = Randomf(0.01f, 3);
		thiz->polyList[i]->targetOpacity = 0.7f;
		thiz->polyCount++;
	}

	// �t�F�C�g�C������
	FadeScreen(FADE_IN_WH, 0, 0.7f);
	
	// BGM���Đ�
	SetVolume(BGM_GAME, -1800);
	PlayBGM(BGM_GAME);

	// �Q�[����ԁ��t�F�C�g���������҂�
	thiz->update = &update_game_fadeWait;
}

// �O���[�o���X�V����
void updateSceneGame(void)
{
	thiz->update();
}

// �I������
void uninitSceneGame(void)
{
	StopSound(BGM_GAME);

	deleteObject(thiz->vignetting);
	deleteObject(thiz->liveUI);
	DeleteSubObj(thiz->scoreUI);
	DeleteSubObj(thiz->timeUI[0]);
	DeleteSubObj(thiz->timeUI[1]);
	DeleteSubObj(thiz->camera);
	DeleteSubObj(thiz->player);
	for (int i = 0; i < thiz->polyCount; i++)
		DeleteSubObj(thiz->polyList[i]);
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if(thiz->enemy[i] != NULL)
			DeleteSubObj(thiz->enemy[i]);
	}

	CleanBullets();
}

// �X�R�A�̎擾
int GetGameScore(void)
{
	return thiz->score;
}

void AddGameScore(int score)
{
	thiz->score += score;
}


/// private�֐�

// �t�@�C�g���������҂�
void update_game_fadeWait(void)
{
	//if (FadeFinished())
	{
		Object_SetActive(thiz->player->base, true);
		Object_SetActive(thiz->camera->base, true);
		thiz->update = &update_game_main;
	}
}

void update_game_main(void)
{
	// �G�̐���
	swapEnemy();

	// �v���C���[�̈ړ�����
	Vector3 &playerPos = thiz->player->base->transform->position;
	if (playerPos.x < -FIELD_RANG_X)
		playerPos.x = -FIELD_RANG_X;
	if (playerPos.x > FIELD_RANG_X)
		playerPos.x = FIELD_RANG_X;
	if (playerPos.y < -FIELD_RANG_Y)
		playerPos.y = -FIELD_RANG_Y;
	if (playerPos.y > FIELD_RANG_Y)
		playerPos.y = FIELD_RANG_Y;

	// �c�@�\���̍X�V
	Polygon_SetPattern(thiz->liveUI->polygon, thiz->player->hp-1);

	// �J�E���g�_�E���X�V
	thiz->timer -= GetDeltaTime();

	// �J�E���g�_�E���\���X�V
	NumberUI_SetNumber(thiz->timeUI[0], (int)(thiz->timer*100)%100);
	NumberUI_SetNumber(thiz->timeUI[1], (int)thiz->timer);

	// �X�R�A�\���X�V
	NumberUI_SetNumber(thiz->scoreUI, thiz->score);

	// �V�[���J�ځ��N���A�V�[��
	if (thiz->timer < 0)
		SetScene(SCENE_CLEAR);

	// �V�[���J�ځ��Q�[���I�[�o�[
	if(thiz->player->hp == 0)
		SetScene(SCENE_GAMEOVER);
}

void swapEnemy(void)
{
	static float timer = 0;

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_E))
		timer = 10;
#endif

	if (timer > 3.0f)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (thiz->enemy[i] == NULL)
			{
				Enemy* enemy = 
				thiz->enemy[i] = NewSubObj(Enemy);

				enemy->target = thiz->player->base->transform;
				enemy->base->transform->position = Vector3(Randomf(-FIELD_RANG_X+500, FIELD_RANG_X-500), Randomf(-FIELD_RANG_Y+500, FIELD_RANG_X-500), 0.0f);

				break;
			}
		}
		timer = 0;
	}

	timer += GetDeltaTime();
}
