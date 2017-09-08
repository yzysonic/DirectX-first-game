#include "SceneGame.h"
#include "Core.h"
#include "NumberUI.h"
#include "Camera.h"
#include "Player.h"
#include "Bullet.h"


typedef struct _SceneGame
{
	Scene base;

	Object* vignetting = NULL;
	Object* liveUI = NULL;
	NumberUI* scoreUI = NULL;
	NumberUI* timeUI[2] = {};
	Camera* camera = NULL;
	Player* player = NULL;

	int score;
	float timer;

	void(*update)(void);
}SceneGame;

// グローバル変数宣言
SceneGame g_SceneGame;
SceneGame *thiz = &g_SceneGame;

// プロトタイプ宣言
void update_game_fadeWait(void);
void update_game_main(void);



/// public関数

// インスタンスの取得
Scene * GetSceneGame(void)
{
	return (Scene*)&g_SceneGame;
}

// 初期化処理
void initSceneGame(void)
{
	thiz->vignetting = newObject(&thiz->vignetting);
	thiz->vignetting->polygon = newPolygon(thiz->vignetting, LAYER_MASK, TEX_VIGNETTING, REND_UI);

	thiz->scoreUI = newNumberUI(5, SCREEN_CENTER_X - 300, 30 - SCREEN_CENTER_Y, TEX_NUMBER, TEX_GAME_SCORE);
	NumberUI_SetOffset(thiz->scoreUI, 130, 0);

	const int x_offset = 35;
	thiz->timeUI[0] = newNumberUI(2, x_offset - SCREEN_CENTER_X, 30 - SCREEN_CENTER_Y, TEX_NUMBER, TEX_GAME_TIME);
	thiz->timeUI[1] = newNumberUI(2, x_offset + 116 - SCREEN_CENTER_X, 30 - SCREEN_CENTER_Y, TEX_NUMBER);
	NumberUI_SetOffset(thiz->timeUI[0], 180, 0);

	thiz->liveUI = newObject(&thiz->liveUI);
	thiz->liveUI->polygon = newPolygon(thiz->liveUI, LAYER_UI_00, TEX_LIFES, REND_UI);
	thiz->liveUI->transform->position = Vector3(x_offset + 3 + GetTexture(TEX_LIFES)->size.x/2 - SCREEN_CENTER_X, 70 - SCREEN_CENTER_Y, 0.0f);

	thiz->player = NewSubObj(Player);
	thiz->camera = newCamera(thiz->player->base->transform, Vector3(0, -50, -5));

	Object_SetActive(thiz->player->base, false);
	Object_SetActive(thiz->camera->base, false);

	Renderer_SetCamera(thiz->camera->base->transform);
	Renderer_SetBackColor(ColorRGBA(210, 210, 210, 255));

	thiz->score = 0;
	thiz->timer = 60.0f;

	FadeScreen(FADE_IN_WH, 0, 0);
	
	SetVolume(BGM_GAME, -1800);
	PlayBGM(BGM_GAME);

	thiz->update = &update_game_fadeWait;
}

// グローバル更新処理
void updateSceneGame(void)
{
	thiz->update();
}

// 終了処理
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

	CleanBullets();
}


/// private関数

// ファイト処理完了待ち
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
	Polygon_SetPattern(thiz->liveUI->polygon, thiz->player->lives-1);
	NumberUI_SetNumber(thiz->timeUI[0], (int)(thiz->timer*100)%100);
	NumberUI_SetNumber(thiz->timeUI[1], (int)thiz->timer);

	thiz->timer -= GetDeltaTime();
}
