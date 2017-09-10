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

// グローバル変数宣言
SceneGame g_SceneGame;
SceneGame *thiz = &g_SceneGame;

// プロトタイプ宣言
void update_game_fadeWait(void);
void update_game_main(void);
void swapEnemy(void);


/// public関数

// インスタンスの取得
Scene * GetSceneGame(void)
{
	return (Scene*)&g_SceneGame;
}

// 初期化処理
void initSceneGame(void)
{
	// 口径食効果
	thiz->vignetting = newObject(&thiz->vignetting);
	thiz->vignetting->polygon = newPolygon(thiz->vignetting, LAYER_MASK, TEX_VIGNETTING, REND_UI);

	// スコアUI
	thiz->scoreUI = newNumberUI(5, SCREEN_CENTER_X - 300, 30 - SCREEN_CENTER_Y, TEX_NUMBER, TEX_GAME_SCORE);
	NumberUI_SetOffset(thiz->scoreUI, 130, 0);

	// タイムUI
	const int x_offset = 35;
	thiz->timeUI[0] = newNumberUI(2, x_offset - SCREEN_CENTER_X, 30 - SCREEN_CENTER_Y, TEX_NUMBER, TEX_GAME_TIME);
	thiz->timeUI[1] = newNumberUI(2, x_offset + 116 - SCREEN_CENTER_X, 30 - SCREEN_CENTER_Y, TEX_NUMBER);
	NumberUI_SetOffset(thiz->timeUI[0], 180, 0);

	// 残機UI 
	thiz->liveUI = newObject(&thiz->liveUI);
	thiz->liveUI->polygon = newPolygon(thiz->liveUI, LAYER_UI_00, TEX_LIFES, REND_UI);
	thiz->liveUI->transform->position = Vector3(x_offset + 3 + GetTexture(TEX_LIFES)->size.x/2 - SCREEN_CENTER_X, 70 - SCREEN_CENTER_Y, 0.0f);

	// プレイヤー
	thiz->player = NewSubObj(Player);
	Object_SetActive(thiz->player->base, false);

	// カメラ
	thiz->camera = newCamera(thiz->player->base->transform, Vector3(0, -50, -5));
	Object_SetActive(thiz->camera->base, false);

	// レンダラー設定
	Renderer_SetCamera(thiz->camera->base->transform);
	Renderer_SetBackColor(ColorRGBA(210, 210, 210, 255));
	Renderer_SetFov(1);

	// ゲームで使う変数
	thiz->score = 0;
	thiz->timer = 60.0f;
	thiz->polyCount = 0;

	for (int i = 0; i < ENEMY_MAX; i++)
		thiz->enemy[i] = NULL;

	// 背景ポリゴン生成
	for (int i = 0; i < GAME_POLY_MAX; i++)
	{
		thiz->polyList[i] = NewSubObj(PolygonElement);
		thiz->polyList[i]->base->transform->position.x = Randomf(-FIELD_RANG_X, FIELD_RANG_X);
		thiz->polyList[i]->base->transform->position.y = Randomf(-FIELD_RANG_Y, FIELD_RANG_Y);
		thiz->polyList[i]->base->transform->position.z = Randomf(0.01f, 3);
		thiz->polyList[i]->targetOpacity = 0.7f;
		thiz->polyCount++;
	}

	// フェイトイン効果
	FadeScreen(FADE_IN_WH, 0, 0.7f);
	
	// BGMを再生
	SetVolume(BGM_GAME, -1800);
	PlayBGM(BGM_GAME);

	// ゲーム状態→フェイト処理完了待ち
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
	for (int i = 0; i < thiz->polyCount; i++)
		DeleteSubObj(thiz->polyList[i]);
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if(thiz->enemy[i] != NULL)
			DeleteSubObj(thiz->enemy[i]);
	}

	CleanBullets();
}

// スコアの取得
int GetGameScore(void)
{
	return thiz->score;
}

void AddGameScore(int score)
{
	thiz->score += score;
}


/// private関数

// ファイト処理完了待ち
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
	// 敵の生成
	swapEnemy();

	// プレイヤーの移動制限
	Vector3 &playerPos = thiz->player->base->transform->position;
	if (playerPos.x < -FIELD_RANG_X)
		playerPos.x = -FIELD_RANG_X;
	if (playerPos.x > FIELD_RANG_X)
		playerPos.x = FIELD_RANG_X;
	if (playerPos.y < -FIELD_RANG_Y)
		playerPos.y = -FIELD_RANG_Y;
	if (playerPos.y > FIELD_RANG_Y)
		playerPos.y = FIELD_RANG_Y;

	// 残機表示の更新
	Polygon_SetPattern(thiz->liveUI->polygon, thiz->player->hp-1);

	// カウントダウン更新
	thiz->timer -= GetDeltaTime();

	// カウントダウン表示更新
	NumberUI_SetNumber(thiz->timeUI[0], (int)(thiz->timer*100)%100);
	NumberUI_SetNumber(thiz->timeUI[1], (int)thiz->timer);

	// スコア表示更新
	NumberUI_SetNumber(thiz->scoreUI, thiz->score);

	// シーン遷移→クリアシーン
	if (thiz->timer < 0)
		SetScene(SCENE_CLEAR);

	// シーン遷移→ゲームオーバー
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
