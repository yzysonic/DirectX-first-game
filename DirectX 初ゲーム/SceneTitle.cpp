#include "SceneTitle.h"
#include "Core.h"
#include "GameManager.h"
#include "PolygonElement.h"

// マクロ定義
#define TitlePolyMax (300)
#define TitleBGMBPS (130)

// アニメーション状態の列挙
enum animeState
{
	Hit,
	Phase1,
	Phase2,
	Wait
};

// 構造体宣言
typedef struct _SceneTitle
{
	Scene base;
	Object *vignetting;
	Object *logo;
	Object *presskey;
	Object *info;
	PolygonElement* polyList[TitlePolyMax];
	int polyCount;
	float timer;
	float polyTimer;
	float logoTimer;
	float keyTimer;
	Timer *beat;
	int beatCount;
	animeState polyState;
	animeState logoState;
	animeState keyState;
	void(*update)(void);
}SceneTitle;


// 内部用関数のプロトタイプ宣言
void update_title_state0(void);
void update_title_state1(void);
void update_title_state2(void);
void update_title_state3(void);
void update_title_poly(void);
void update_title_logo(void);
void update_title_presskey(void);

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
	thiz->update = &update_title_state0;

	thiz->vignetting = newObject(Obj_Object, &thiz->vignetting);
	thiz->vignetting->polygon = newPolygon(thiz->vignetting, LAYER_UI_02, TEX_VIGNETTING);

	thiz->timer = 0;

	// フェイトイン（黒=>白）
	FadeScreen(FADE_IN_BK, 0, 1.5f);
}

// グローバル更新処理
void updateSceneTitle(void)
{
	if (Timer_ElapsedTime(thiz->beat) >= (60.0f/TitleBGMBPS)*(thiz->beatCount + 1))
	{
		while(Timer_ElapsedTime(thiz->beat) >= (60.0f / TitleBGMBPS)*(thiz->beatCount + 1))
			thiz->beatCount++;

		if((thiz->beatCount % 2) == 0)
			thiz->logoState = Hit;
		if ((thiz->beatCount % 2) == 0)
			thiz->keyState = Hit;
		if((thiz->beatCount % 8 ) == 0)
			thiz->polyState = Hit;
	}
	g_SceneTitle.update();
}

// 終了処理
void uninitSceneTitle(void)
{
	deleteObject(thiz->logo);
	deleteObject(thiz->presskey);
	deleteObject(thiz->info);
	deleteObject(thiz->vignetting);

	SafeDelete(thiz->beat);

	for(int i=0;i<thiz->polyCount;i++)
		DeleteObj(g_SceneTitle.polyList[i]);;

	StopSound(BGM_TITLE);
}


/// private関数

//----------------------
//各状態の更新処理
//----------------------

// フェイト処理完了待ち
void update_title_state0(void)
{
	// 処理完了
	if (FadeFinished())
	{
		// BGM再生
		SetVolume(BGM_TITLE, -1800);
		PlayBGM(BGM_TITLE);
		Timer_Reset(thiz->beat);

		// 状態遷移
		thiz->update = &update_title_state1;
	}
}

// 背景ポリゴンの生成
void update_title_state1(void)
{
	// アニメーションスキップ
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		for (int i = 0; thiz->polyCount<TitlePolyMax; i++)
		{
			PolygonElement *poly = NewObj(PolygonElement);
			thiz->polyList[thiz->polyCount++] = poly;
		}

	}

	// 生成アニメーション処理
	else if (thiz->polyCount < TitlePolyMax)
	{
		for (int i = 0; i < 3; i++)
		{
			PolygonElement *poly = NewObj(PolygonElement);
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
		thiz->logoTimer = 0;
		Polygon_SetOpacity(thiz->logo->polygon, 0);

		// 状態遷移
		thiz->update = &update_title_state2;
	}
}

// ロゴアニメーション
void update_title_state2(void)
{
	// アニメーション処理
	const float interval = 0.3f;
	if (thiz->logoTimer <= interval)
	{
		float progress = thiz->logoTimer / interval;
		thiz->logo->transform->scale.x = Lerpf(3, 1, progress);
		thiz->logo->transform->scale.y = Lerpf(3, 1, progress);
		Polygon_SetOpacity(thiz->logo->polygon, progress);
		thiz->logoTimer += GetDeltaTime();
	}

	// 処理完了
	else
	{
		thiz->logo->transform->scale = Vector3(1.0f, 1.0f, 1.0f);

		// PRESS KEY 表示
		thiz->presskey = newObject(Obj_Object, &thiz->presskey);
		thiz->presskey->polygon = newPolygon(thiz->presskey, LAYER_UI_00, TEX_TITLE_PRESSKEY);
		thiz->presskey->transform->position = Vector3(0, 550 - SCREEN_CENTER_Y, 0);

		// 作者情報表示
		thiz->info = newObject(Obj_Object, &thiz->info);
		thiz->info->polygon = newPolygon(thiz->info, LAYER_UI_00, TEX_TITLE_INFO);
		thiz->info->transform->position.x = SCREEN_CENTER_X - thiz->info->polygon->pTexture->size.x / 2 - 30;
		thiz->info->transform->position.y = SCREEN_CENTER_Y - thiz->info->polygon->pTexture->size.y - 10;
		thiz->info->transform->position.z = 0;

		thiz->logoState = Wait;
		thiz->keyState = Wait;
		thiz->polyState = Wait;

		// 状態遷移
		thiz->update = &update_title_state3;
	}

}

// 入力待ち
void update_title_state3(void)
{

	// メニュー表示
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetScene(SCENE_TEST);
		return;
	}

	// 各アニメーション処理
	update_title_poly();
	update_title_logo();
	update_title_presskey();
}


//----------------------
//各ループアニメーション処理
//----------------------

// 背景ポリゴン
void update_title_poly(void)
{
	switch (thiz->polyState)
	{
	case Hit:
		for (int i = 0; i < thiz->polyCount; i++)
		{
			thiz->polyList[i]->prePos = thiz->polyList[i]->base->transform->position;
			thiz->polyList[i]->nextColor = ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 200);
		}
		thiz->polyState = Phase1;
		thiz->polyTimer = 0;
		break;
	case Phase1:
	{
		float interval = 2.0f;
		if (thiz->polyTimer <= interval)
		{
			float progress = thiz->polyTimer / interval;
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
		thiz->polyTimer += GetDeltaTime();
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
void update_title_logo(void)
{
	switch (thiz->logoState)
	{
	case Hit:
		thiz->logoState = Phase1;
		thiz->logoTimer = 0;
		break;
	case Phase1:
	{
		float interval = 0.05f;
		if (thiz->logoTimer <= interval)
		{
			float porgress = thiz->logoTimer / interval;
			thiz->logo->transform->scale = Vector3(1, 1, 0)*Lerpf(1.0f, 1.15f, porgress);
		}
		else
		{
			thiz->logoState = Phase2;
			thiz->logoTimer = 0;
		}
		thiz->logoTimer += GetDeltaTime();
		break;
	}
	case Phase2:
		thiz->logo->transform->scale = Vector3(1, 1, 0)*Lerpf(thiz->logo->transform->scale.x, 1.0f, GetDeltaTime()*3.0f);
		break;
	}

}

// PRESS KEY
void update_title_presskey(void)
{
	switch (thiz->keyState)
	{
	case Hit:
		thiz->keyState = Phase1;
		thiz->keyTimer = 0;
		break;
	case Phase1:
	{
		float interval = 0.05f;
		if (thiz->keyTimer <= interval)
		{
			float porgress = thiz->keyTimer / interval;
			Polygon_SetOpacity(thiz->presskey->polygon, Lerpf(0.95f, 0.5f, porgress));
		}
		else
		{
			thiz->keyState = Phase2;
			thiz->keyTimer = 0;
		}
		thiz->keyTimer += GetDeltaTime();
		break;
	}
	case Phase2:
		Polygon_SetOpacity(thiz->presskey->polygon, Lerpf(Polygon_GetOpacity(thiz->presskey->polygon), 0.95f, GetDeltaTime()*3.0f));
		break;
	}

}
