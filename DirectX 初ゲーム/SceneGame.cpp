#include "SceneGame.h"
#include "Core.h"
#include "Camera.h"
#include "Player.h"


typedef struct _SceneGame
{
	Scene base;

	Camera* camera = NULL;
	Player* player = NULL;

	void(*update)(void);
}SceneGame;

// �O���[�o���ϐ��錾
SceneGame g_SceneGame;
SceneGame *thiz = &g_SceneGame;

// �v���g�^�C�v�錾
void update_game_fadeWait(void);
void update_game_main(void);



/// public�֐�

// �C���X�^���X�̎擾
Scene * GetSceneGame(void)
{
	return (Scene*)&g_SceneGame;
}

// ����������
void initSceneGame(void)
{
	thiz->player = NewSubObj(Player);
	thiz->camera = newCamera(thiz->player->base->transform, Vector3(0, -50, -5));

	Object_SetActive(thiz->player->base, false);
	Object_SetActive(thiz->camera->base, false);

	Renderer_SetCamera(thiz->camera->base->transform);

	FadeScreen(FADE_IN_WH, 0, 1);

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
	DeleteSubObj(thiz->camera);
	DeleteSubObj(thiz->player);
}


/// private�֐�

// �t�@�C�g���������҂�
void update_game_fadeWait(void)
{
	if (FadeFinished())
	{
		Object_SetActive(thiz->player->base, true);
		Object_SetActive(thiz->camera->base, true);
		thiz->update = &update_game_main;
	}
}

void update_game_main(void)
{
}
