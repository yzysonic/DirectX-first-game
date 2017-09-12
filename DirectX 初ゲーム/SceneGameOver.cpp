#include "SceneGameOver.h"
#include "Core.h"
#include "GameManager.h"

typedef struct _SceneGameOver
{
	Scene base;
	Object *bk;
	float timer;
	void(*update)(void);
}SceneGameOver;

// グローバル変数宣言
static SceneGameOver g_SceneGameOver;
static SceneGameOver *thiz = &g_SceneGameOver;

// プロトタイプ宣言
void update_gameover_fadeWait(void);
void update_gameover_state0(void);
void update_gameover_state1(void);

Scene * GetSceneGameOver(void)
{
	return (Scene*)&g_SceneGameOver;
}

void initSceneGameOver(void)
{
	thiz->bk = newObject(&thiz->bk);
	thiz->bk->polygon = newPolygon(thiz->bk, LAYER_BG_00, TEX_GAME_OVER, REND_UI);
	thiz->timer = 0;

	// BGMを再生
	SetVolume(BGM_GAMEOVER, -1800);
	PlayBGM(BGM_GAMEOVER);

	// フェイトイン効果
	FadeScreen(FADE_IN_WH);

	thiz->update = &update_gameover_fadeWait;
}

void updateSceneGameOver(void)
{
	thiz->update();
	thiz->timer += GetDeltaTime();
}

void uninitSceneGameOver(void)
{
	deleteObject(thiz->bk);
}

void update_gameover_fadeWait(void)
{
	if (FadeFinished())
	{
		thiz->update = &update_gameover_state0;
	}
}

void update_gameover_state0(void)
{
	if (thiz->timer > 4.0f)
	{
		FadeScreen(FADE_OUT_BK, 0, 0.7f);
		thiz->timer = 0;
		thiz->update = &update_gameover_state1;
	}
		
}

void update_gameover_state1(void)
{
	if (thiz->timer > 1.0f)
	{
		StopSound(BGM_GAMEOVER);
		SetScene(SCENE_TITLE);
	}
}
