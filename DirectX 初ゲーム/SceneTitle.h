#pragma once

#include "Core/Scene.h"
#include "Core/Core.h"
#include "TitleCamera.h"
#include "TitlePoly.h"
#include "FieldPolygon.h"

class SceneTitle : public Scene
{
	static constexpr UINT TitlePolyMax = 12000;
	static constexpr UINT TitleBGMBPS = 130;

public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

private:
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

	// カメラ
	TitleCamera *camera;

	// 画像
	Object *vignetting = nullptr;
	Object *logo = nullptr;
	Object *presskey = nullptr;
	Object *info = nullptr;
	Object *cursor = nullptr;
	Object *start = nullptr;
	Object *exit = nullptr;

	// 背景ポリゴン
	TitlePoly *title_poly = nullptr;
	FieldPolygon *field_poly = nullptr;

	// 時間計測
	float timer;
	Timer *beat = nullptr;
	int beatCount;

	// アニメーション状態
	AnimeState polyState;
	AnimeState logoState;
	AnimeState keyState;

	// カーソル
	int cursorPos;

	Command com;
	bool playBack;

	// 状態ポインタ
	void(SceneTitle::*pUpdate)(void);

	void syncAnimation(void);
	void update_fadeWait(void);
	void update_createPoly(void);
	void update_showLogo(void);
	void update_pressWait(void);
	void update_showMenu_A(void);
	void update_showMenu_B(void);
	void update_menu(void);
	void update_zoomWait(void);

	void update_anime_poly(void);
	void update_anime_logo(void);
	void update_anime_presskey(void);

};
