#include "SceneClear.h"
#include "GameManager.h"
#include "Core.h"
#include "SceneGame.h"
#include "NumberUI.h"

typedef struct _SceneClear
{
	Scene base;
	Object *bk;
	NumberUI *score;
	float timer;
	void(*update)(void);
}SceneClear;

// グローバル変数宣言
static SceneClear g_SceneClear;
static SceneClear *thiz = &g_SceneClear;

// プロトタイプ宣言
void update_clear_fadeWait(void);
void update_clear_state0(void);
void update_clear_state1(void);


Scene * GetSceneClear(void)
{
	return (Scene*)&g_SceneClear;
}


void initSceneClear(void)
{
	thiz->bk = newObject(&thiz->bk);
	thiz->bk->polygon = newPolygon(thiz->bk, LAYER_BG_00, TEX_CLEAR, REND_UI);
	thiz->score = newNumberUI(5, -120, 0, TEX_NUMBER, TEX_GAME_SCORE);
	NumberUI_SetOffset(thiz->score, 130, 0);
	NumberUI_SetNumber(thiz->score, GetGameScore());
	thiz->timer = 0;

	// BGMを再生
	SetVolume(BGM_CLEAR, -1800);
	PlayBGM(BGM_CLEAR);

	// フェイトイン効果
	FadeScreen(FADE_IN_WH);

	thiz->update = &update_clear_fadeWait;
}

void updateSceneClear(void)
{
	thiz->update();
	thiz->timer += GetDeltaTime();
}

void uninitSceneClear(void)
{
	DeleteSubObj(thiz->score);
	deleteObject(thiz->bk);
}

void update_clear_fadeWait(void)
{
	if (FadeFinished())
	{
		thiz->update = &update_clear_state0;
	}
}

void update_clear_state0(void)
{
	if (thiz->timer > 4.0f)
	{
		FadeScreen(FADE_OUT_BK, 0, 0.7f);
		thiz->timer = 0;
		thiz->update = &update_clear_state1;
	}
		
}

void update_clear_state1(void)
{
	if (thiz->timer > 1.0f)
	{
		StopSound(BGM_CLEAR);
		SetScene(SCENE_TITLE);
	}
		
}
