#include "SceneTitle.h"
#include "Core.h"
#include "Game.h"
#include "GameManager.h"
#include "Renderer.h"
#include "PolygonElement.h"

// �}�N����`
#define TitlePolyMax (700)
#define TitleBGMBPS (130)

// �A�j���[�V������Ԃ̗�
enum AnimeState
{
	Hit,
	Phase1,
	Phase2,
	Wait
};

enum Command
{
	COM_START,
	COM_EXIT,
	COM_WAIT,
	COM_OP,
};

// �\���̐錾
typedef struct _SceneTitle
{
	Scene base;

	// �摜
	Object *vignetting;
	Object *logo;
	Object *presskey;
	Object *info;
	Object *cursor;
	Object *start;
	Object *exit;

	// �w�i�|���S��
	PolygonElement* polyList[TitlePolyMax];
	int polyCount;

	// ���Ԍv��
	float timer;
	Timer *beat;
	int beatCount;

	// �A�j���[�V�������
	AnimeState polyState;
	AnimeState logoState;
	AnimeState keyState;

	// �J�[�\��
	int cursorPos;

	Command com;

	// ��ԃ|�C���^
	void(*update)(void);

}SceneTitle;


// �����p�֐��̃v���g�^�C�v�錾
void syncAnimation(void);
void update_title_waitFade(void);
void update_title_createPoly(void);
void update_title_showLogo(void);
void update_title_pressWait(void);
void update_title_showMenu_A(void);
void update_title_showMenu_B(void);
void update_title_menu(void);

void update_title_anime_poly(void);
void update_title_anime_logo(void);
void update_title_anime_presskey(void);

// �O���[�o���ϐ��錾
SceneTitle g_SceneTitle;
SceneTitle* thiz = &g_SceneTitle;


/// public�֐�

// �V�[���C���X�^���X�̎擾
Scene * GetSceneTitle(void)
{
	return (Scene*)&g_SceneTitle;
}

// ������
void initSceneTitle(void)
{
	thiz->polyCount = 0;
	thiz->beat = newTimer();
	thiz->beatCount = 0;
	thiz->timer = 0;
	thiz->com = COM_WAIT;

	thiz->vignetting = newObject(Obj_Object, &thiz->vignetting);
	thiz->vignetting->polygon = newPolygon(thiz->vignetting, LAYER_UI_02, TEX_VIGNETTING);

	// �A�j���[�V������ԏ�����
	thiz->logoState =
	thiz->keyState =
	thiz->polyState = Wait;


	// �w�i�F�ݒ�
	Renderer_SetBackColor(D3DCOLOR_RGBA(243, 242, 238, 255));

	// �J�����̏�����
	Renderer_SetCamera(NULL);
	Renderer_SetFov(0.0f);

	// �t�F�C�g�A�E�g
	FadeScreen(FADE_OUT_BK, 0, 0);

	// �������
	thiz->update = &update_title_waitFade;
}

// �O���[�o���X�V����
void updateSceneTitle(void)
{
	// �e�A�j���[�V��������
	update_title_anime_poly();
	update_title_anime_logo();

	// ���[�J���X�V����
	g_SceneTitle.update();

	thiz->timer += GetDeltaTime();
}

// �I������
void uninitSceneTitle(void)
{
	DeleteObj(thiz->cursor);
	DeleteObj(thiz->start);
	DeleteObj(thiz->exit);
	DeleteObj(thiz->logo);
	DeleteObj(thiz->presskey);
	DeleteObj(thiz->info);
	DeleteObj(thiz->vignetting);

	SafeDelete(thiz->beat);

	for(int i=0;i<thiz->polyCount;i++)
		DeleteSubObj(g_SceneTitle.polyList[i]);;

	StopSound(BGM_TITLE);
}


/// private�֐�

// �A�j���[�V������BGM�̓�������
void syncAnimation(void)
{
	if (Timer_ElapsedTime(thiz->beat) >= (60.0f / TitleBGMBPS)*(thiz->beatCount + 1))
	{
		while (Timer_ElapsedTime(thiz->beat) >= (60.0f / TitleBGMBPS)*(thiz->beatCount + 1))
			thiz->beatCount++;

		if ((thiz->beatCount % 2) == 0)
			thiz->logoState = Hit;
		if ((thiz->beatCount % 2) == 0)
			thiz->keyState = Hit;
		if ((thiz->beatCount % 8) == 0)
			thiz->polyState = Hit;
	}
}


//----------------------
//�e��Ԃ̍X�V����
//----------------------

// �t�F�C�g���������҂�
void update_title_waitFade(void)
{
	// ��������
	if (FadeFinished())
	{
		switch (thiz->com)
		{
		case COM_WAIT:
			if (thiz->timer > 1.0f)
			{
				thiz->com = COM_OP;
				// �t�F�C�g�C��
				FadeScreen(FADE_IN_BK, 0, 0.7f);
			}
			thiz->timer += GetDeltaTime();
			break;

		case COM_OP:
			// BGM�Đ�
			SetVolume(BGM_TITLE, -1800);
			PlayBGM(BGM_TITLE);
			Timer_Reset(thiz->beat);

			// ��ԑJ��
			thiz->update = &update_title_createPoly;
			break;
		
		case COM_START:
			// �V�[���J��
			SetScene(SCENE_TEST);
			break;

		case COM_EXIT:
			// �Q�[���I��
			StopGame();
			break;
		}
	}
}

// �w�i�|���S���̐���
void update_title_createPoly(void)
{
	// �A�j���[�V�����X�L�b�v
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		for (int i = 0; thiz->polyCount<TitlePolyMax; i++)
		{
			PolygonElement *poly = NewSubObj(PolygonElement);
			thiz->polyList[thiz->polyCount++] = poly;
		}

	}

	// �����A�j���[�V��������
	else if (thiz->polyCount < TitlePolyMax)
	{
		for (int i = 0; i < 10; i++)
		{
			PolygonElement *poly = NewSubObj(PolygonElement);
			thiz->polyList[thiz->polyCount++] = poly;
			if (thiz->polyCount == TitlePolyMax)
				break;
		}
	}

	// ��������
	else 
	{
		// ���S�\��
		thiz->logo = newObject(Obj_Object, &thiz->logo);
		thiz->logo->polygon = newPolygon(thiz->logo, LAYER_UI_00, TEX_TITLE_LOGO);
		thiz->logo->transform->position = Vector3(0, 200 - SCREEN_CENTER_Y, 0);
		thiz->timer = 0;
		Polygon_SetOpacity(thiz->logo->polygon, 0);

		// ��ԑJ��
		thiz->update = &update_title_showLogo;
	}
}

// ���S�A�j���[�V����
void update_title_showLogo(void)
{
	// �A�j���[�V��������
	const float interval = 0.3f;
	if (thiz->timer <= interval)
	{
		float progress = thiz->timer / interval;
		thiz->logo->transform->scale.x = Lerpf(3, 1, progress);
		thiz->logo->transform->scale.y = Lerpf(3, 1, progress);
		Polygon_SetOpacity(thiz->logo->polygon, progress);
	}

	// ��������
	else
	{
		thiz->logo->transform->scale = Vector3(1.0f, 1.0f, 1.0f);

		// PRESS KEY �\��
		thiz->presskey = newObject(Obj_Object, &thiz->presskey);
		thiz->presskey->polygon = newPolygon(thiz->presskey, LAYER_UI_00, TEX_TITLE_PRESSKEY);
		thiz->presskey->transform->position = Vector3(0, 475 - SCREEN_CENTER_Y, 0);

		// ��ҏ��\��
		thiz->info = newObject(Obj_Object, &thiz->info);
		thiz->info->polygon = newPolygon(thiz->info, LAYER_UI_02, TEX_TITLE_INFO);
		thiz->info->transform->position.x = SCREEN_CENTER_X - thiz->info->polygon->pTexture->size.x / 2 - 30;
		thiz->info->transform->position.y = SCREEN_CENTER_Y - thiz->info->polygon->pTexture->size.y - 10;
		thiz->info->transform->position.z = 0;

		thiz->logoState = Wait;
		thiz->keyState = Wait;
		thiz->polyState = Wait;

		// ��ԑJ��
		thiz->update = &update_title_pressWait;
	}

}

// ���͑҂�
void update_title_pressWait(void)
{
	syncAnimation();

	// ���j���[�\��
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		thiz->timer = 0;

		// ��ԑJ��
		thiz->update = &update_title_showMenu_A;
	}

	update_title_anime_presskey();
}

// ���j���[�A�j���[�V����A
void update_title_showMenu_A(void)
{
	syncAnimation();
	Renderer_SetFov(Lerpf(Renderer_GetFov(), 1.0f, GetDeltaTime() * 10));

	const float interval = 0.1f;

	float progress = thiz->timer / interval;
	if (thiz->timer <= interval+0.1f)
	{
		thiz->presskey->transform->scale = LerpV3(Vector3(1, 1, 0), Vector3(1.3f, 0.8f, 0), progress);
		Polygon_SetOpacity(thiz->presskey->polygon, Lerpf(1, 0.0f, progress));
	}
	else
	{
		// ���j���[�\��
		thiz->cursor = newObject(Obj_Object, &thiz->cursor);
		thiz->cursor->polygon = newPolygon(thiz->cursor, LAYER_UI_00, TEX_TITLE_CURSOR);
		thiz->cursor->transform->position = Vector3(-80, 415 - SCREEN_CENTER_Y, 0);
		Polygon_SetOpacity(thiz->cursor->polygon, 0);

		thiz->start = newObject(Obj_Object, &thiz->start);
		thiz->start->polygon = newPolygon(thiz->start, LAYER_UI_00, TEX_TITLE_START);
		thiz->start->transform->position = Vector3(40, 415 - SCREEN_CENTER_Y, 0);
		Polygon_SetOpacity(thiz->start->polygon, 0);

		thiz->exit = newObject(Obj_Object, &thiz->exit);
		thiz->exit->polygon = newPolygon(thiz->exit, LAYER_UI_00, TEX_TITLE_EXIT);
		thiz->exit->transform->position = Vector3(40, 500 - SCREEN_CENTER_Y, 0);
		Polygon_SetOpacity(thiz->exit->polygon, 0);

		thiz->cursorPos = 0;

		// ��ԑJ��
		thiz->update = &update_title_showMenu_B;
	}
}

// ���j���[�A�j���[�V����B
void update_title_showMenu_B(void)
{
	syncAnimation();
	Renderer_SetFov(Lerpf(Renderer_GetFov(), 1.0f, GetDeltaTime() * 10));


	const float interval = 0.5f;

	float progress = thiz->timer / interval;
	if (thiz->timer <= interval + 0.1f)
	{
		Polygon_SetOpacity(thiz->cursor->polygon, Lerpf(0, 1, progress));
		Polygon_SetOpacity(thiz->start->polygon, Lerpf(0, 1, progress));
		Polygon_SetOpacity(thiz->exit->polygon, Lerpf(0, 1, progress));
	}
	else
	{
		// ��ԑJ��
		thiz->update = &update_title_menu;
	}
}

// ���j���[�X�V����
void update_title_menu(void)
{
	syncAnimation();

	// ���j���[����
	if (GetKeyboardTrigger(DIK_UP))
	{
		if (thiz->cursorPos > 0)
		{
			thiz->cursor->transform->position.y -= 85;
			thiz->cursorPos--;
		}
	}
	if (GetKeyboardTrigger(DIK_DOWN))
	{
		if (thiz->cursorPos < 1)
		{
			thiz->cursor->transform->position.y += 85;
			thiz->cursorPos++;
		}
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		thiz->com = (Command)thiz->cursorPos;

		switch (thiz->com)
		{
		case COM_START:
			FadeScreen(FADE_OUT_WH, 0);
			break;
		case COM_EXIT:
			FadeScreen(FADE_OUT_BK, 0);
			break;
		}

		// ��ԑJ��
		thiz->update = &update_title_waitFade;
	}

}


//----------------------
//�e���[�v�A�j���[�V��������
//----------------------

// �w�i�|���S��
void update_title_anime_poly(void)
{
	static float timer;
	switch (thiz->polyState)
	{
	case Hit:
		for (int i = 0; i < thiz->polyCount; i++)
		{
			thiz->polyList[i]->prePos = thiz->polyList[i]->base->transform->position;
			thiz->polyList[i]->nextColor = ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 200);
		}
		thiz->polyState = Phase1;
		timer = 0;
		break;
	case Phase1:
	{
		float interval = 2.0f;
		if (timer <= interval)
		{
			float progress = timer / interval;
			for (int i = 0; i < thiz->polyCount; i++)
			{
				//Vector3 dir;
				//D3DXVec3Normalize(&dir, &thiz->polyList[i]->prePos);
				//thiz->polyList[i]->base->transform->position += dir*10.0f*GetDeltaTime() / interval;

				//thiz->polyList[i]->base->transform->scale = Vector3(1, 1, 0)*Lerpf(0.4f, 0.6f, progress);

				Polygon_SetColor(thiz->polyList[i]->base->polygon, LerpC(thiz->polyList[i]->base->polygon->color, thiz->polyList[i]->nextColor, progress));
			}
		}
		else
		{
			thiz->polyState = Wait;
			//thiz->polyState = Phase2;
		}
		timer += GetDeltaTime();
		break;
	}
	case Phase2:

		for (int i = 0; i < thiz->polyCount; i++)
		{
			//thiz->polyList[i]->base->transform->position = LerpV3(thiz->polyList[i]->base->transform->position, thiz->polyList[i]->prePos, GetDeltaTime()*10.0f);

			thiz->polyList[i]->base->transform->scale = Vector3(1, 1, 0)*Lerpf(thiz->polyList[i]->base->transform->scale.x, 0.4f, GetDeltaTime()*3.0f);
		}


		break;
	}

}

// ���S
void update_title_anime_logo(void)
{
	static float timer;
	switch (thiz->logoState)
	{
	case Hit:
		thiz->logoState = Phase1;
		timer = 0;
		break;
	case Phase1:
	{
		float interval = 0.05f;
		if (timer <= interval)
		{
			float porgress = timer / interval;
			thiz->logo->transform->scale = Vector3(1, 1, 0)*Lerpf(1.0f, 1.13f, porgress);
		}
		else
		{
			thiz->logoState = Phase2;
			timer = 0;
		}
		timer += GetDeltaTime();
		break;
	}
	case Phase2:
		thiz->logo->transform->scale = Vector3(1, 1, 0)*Lerpf(thiz->logo->transform->scale.x, 1.0f, GetDeltaTime()*3.0f);
		break;
	}

}

// PRESS KEY
void update_title_anime_presskey(void)
{
	static float timer;
	switch (thiz->keyState)
	{
	case Hit:
		thiz->keyState = Phase1;
		timer = 0;
		break;
	case Phase1:
	{
		float interval = 0.05f;
		if (timer <= interval)
		{
			float porgress = timer / interval;
			Polygon_SetOpacity(thiz->presskey->polygon, Lerpf(0.95f, 0.5f, porgress));
		}
		else
		{
			thiz->keyState = Phase2;
			timer = 0;
		}
		timer += GetDeltaTime();
		break;
	}
	case Phase2:
		Polygon_SetOpacity(thiz->presskey->polygon, Lerpf(Polygon_GetOpacity(thiz->presskey->polygon), 0.95f, GetDeltaTime()*3.0f));
		break;
	}

}
