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
	thiz->player = NewSubObj(Player);
	thiz->camera = newCamera(thiz->player->base->transform, Vector3(0, -50, -5));

	Object_SetActive(thiz->player->base, false);
	Object_SetActive(thiz->camera->base, false);

	Renderer_SetCamera(thiz->camera->base->transform);

	FadeScreen(FADE_IN_WH, 0, 1);

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
	DeleteSubObj(thiz->camera);
	DeleteSubObj(thiz->player);
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
}
