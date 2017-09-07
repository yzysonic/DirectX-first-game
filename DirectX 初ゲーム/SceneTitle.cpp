#include "SceneTitle.h"
#include "Core.h"
#include "Game.h"
#include "GameManager.h"
#include "Renderer.h"
#include "PolygonElement.h"

// マクロ定義
#define TitlePolyMax (700)
#define TitleBGMBPS (130)

// アニメーション状態の列挙
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

// 構造体宣言
typedef struct _SceneTitle
{
	Scene base;

	// 画像
	Object *vignetting;
	Object *logo;
	Object *presskey;
	Object *info;
	Object *cursor;
	Object *start;
	Object *exit;

	// 背景ポリゴン
	PolygonElement* polyList[TitlePolyMax];
	int polyCount;

	// 時間計測
	float timer;
	Timer *beat;
	int beatCount;

	// アニメーション状態
	AnimeState polyState;
	AnimeState logoState;
	AnimeState keyState;

	// カーソル
	int cursorPos;

	Command com;

	// 状態ポインタ
	void(*update)(void);

}SceneTitle;


// 内部用関数のプロトタイプ宣言
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

// グローバル変数宣言
SceneTitle g_SceneTitle;
SceneTitle* thiz = &g_SceneTitle;


/// public関数

// シーンインスタンスの取得
Scene * GetSceneTitle(void)
{
	return (Scene*)&g_SceneTitle;
}

// 初期化
void initSceneTitle(void)
{
	thiz->polyCount = 0;
	thiz->beat = newTimer();
	thiz->beatCount = 0;
	thiz->timer = 0;
	thiz->com = COM_WAIT;

	thiz->vignetting = newObject(Obj_Object, &thiz->vignetting);
	thiz->vignetting->polygon = newPolygon(thiz->vignetting, LAYER_UI_02, TEX_VIGNETTING);

	// アニメーション状態初期化
	thiz->logoState =
	thiz->keyState =
	thiz->polyState = Wait;


	// 背景色設定
	Renderer_SetBackColor(D3DCOLOR_RGBA(243, 242, 238, 255));

	// カメラの初期化
	Renderer_SetCamera(NULL);
	Renderer_SetFov(0.0f);

	// フェイトアウト
	FadeScreen(FADE_OUT_BK, 0, 0);

	// 初期状態
	thiz->update = &update_title_waitFade;
}

// グローバル更新処理
void updateSceneTitle(void)
{
	// 各アニメーション処理
	update_title_anime_poly();
	update_title_anime_logo();

	// ローカル更新処理
	g_SceneTitle.update();

	thiz->timer += GetDeltaTime();
}

// 終了処理
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


/// private関数

// アニメーションとBGMの同期処理
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
//各状態の更新処理
//----------------------

// フェイト処理完了待ち
void update_title_waitFade(void)
{
	// 処理完了
	if (FadeFinished())
	{
		switch (thiz->com)
		{
		case COM_WAIT:
			if (thiz->timer > 1.0f)
			{
				thiz->com = COM_OP;
				// フェイトイン
				FadeScreen(FADE_IN_BK, 0, 0.7f);
			}
			thiz->timer += GetDeltaTime();
			break;

		case COM_OP:
			// BGM再生
			SetVolume(BGM_TITLE, -1800);
			PlayBGM(BGM_TITLE);
			Timer_Reset(thiz->beat);

			// 状態遷移
			thiz->update = &update_title_createPoly;
			break;
		
		case COM_START:
			// シーン遷移
			SetScene(SCENE_TEST);
			break;

		case COM_EXIT:
			// ゲーム終了
			StopGame();
			break;
		}
	}
}

// 背景ポリゴンの生成
void update_title_createPoly(void)
{
	// アニメーションスキップ
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		for (int i = 0; thiz->polyCount<TitlePolyMax; i++)
		{
			PolygonElement *poly = NewSubObj(PolygonElement);
			thiz->polyList[thiz->polyCount++] = poly;
		}

	}

	// 生成アニメーション処理
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

	// 処理完了
	else 
	{
		// ロゴ表示
		thiz->logo = newObject(Obj_Object, &thiz->logo);
		thiz->logo->polygon = newPolygon(thiz->logo, LAYER_UI_00, TEX_TITLE_LOGO);
		thiz->logo->transform->position = Vector3(0, 200 - SCREEN_CENTER_Y, 0);
		thiz->timer = 0;
		Polygon_SetOpacity(thiz->logo->polygon, 0);

		// 状態遷移
		thiz->update = &update_title_showLogo;
	}
}

// ロゴアニメーション
void update_title_showLogo(void)
{
	// アニメーション処理
	const float interval = 0.3f;
	if (thiz->timer <= interval)
	{
		float progress = thiz->timer / interval;
		thiz->logo->transform->scale.x = Lerpf(3, 1, progress);
		thiz->logo->transform->scale.y = Lerpf(3, 1, progress);
		Polygon_SetOpacity(thiz->logo->polygon, progress);
	}

	// 処理完了
	else
	{
		thiz->logo->transform->scale = Vector3(1.0f, 1.0f, 1.0f);

		// PRESS KEY 表示
		thiz->presskey = newObject(Obj_Object, &thiz->presskey);
		thiz->presskey->polygon = newPolygon(thiz->presskey, LAYER_UI_00, TEX_TITLE_PRESSKEY);
		thiz->presskey->transform->position = Vector3(0, 475 - SCREEN_CENTER_Y, 0);

		// 作者情報表示
		thiz->info = newObject(Obj_Object, &thiz->info);
		thiz->info->polygon = newPolygon(thiz->info, LAYER_UI_02, TEX_TITLE_INFO);
		thiz->info->transform->position.x = SCREEN_CENTER_X - thiz->info->polygon->pTexture->size.x / 2 - 30;
		thiz->info->transform->position.y = SCREEN_CENTER_Y - thiz->info->polygon->pTexture->size.y - 10;
		thiz->info->transform->position.z = 0;

		thiz->logoState = Wait;
		thiz->keyState = Wait;
		thiz->polyState = Wait;

		// 状態遷移
		thiz->update = &update_title_pressWait;
	}

}

// 入力待ち
void update_title_pressWait(void)
{
	syncAnimation();

	// メニュー表示
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		thiz->timer = 0;

		// 状態遷移
		thiz->update = &update_title_showMenu_A;
	}

	update_title_anime_presskey();
}

// メニューアニメーションA
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
		// メニュー表示
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

		// 状態遷移
		thiz->update = &update_title_showMenu_B;
	}
}

// メニューアニメーションB
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
		// 状態遷移
		thiz->update = &update_title_menu;
	}
}

// メニュー更新処理
void update_title_menu(void)
{
	syncAnimation();

	// メニュー操作
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

		// 状態遷移
		thiz->update = &update_title_waitFade;
	}

}


//----------------------
//各ループアニメーション処理
//----------------------

// 背景ポリゴン
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

// ロゴ
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
