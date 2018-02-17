#pragma once
#include "Core/Scene.h"
#include "Core/Core.h"
#include "NumberUI.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "FieldPolygon.h"
#include "MiniMap.h"
#include "LiveUI.h"
#include "ScoreUI.h"
#include "FieldBoundary.h"

class SceneGame : public Scene
{
public:
	static constexpr float FieldRangX = 1280 / 2 + 500;
	static constexpr float FieldRangY = 720 / 2 + 500;
	static constexpr float GameTime = 60.0f;
	static constexpr float ShowBoundaryMargin = 150.0f;
	static constexpr int EnemyMax = 5;

public:
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;
	void OnPause(void) override;
	void OnResume(void) override;
	int getGameScore(void);
	void addGameScore(int score);
	void player_limit(void);

private:
	enum NextState 
	{
		Start,
		GameOver,
		GameClear
	} next_state;
	Object* vignetting = nullptr;
	LiveUI* liveUI = nullptr;
	ScoreUI* scoreUI = nullptr;
	NumberUI* timeUI[2] = {};
	Camera* camera;
	FieldPolygon* field_poly;
	Player* player = nullptr;
	Enemy* enemy[EnemyMax] = {};
	MiniMap* minimap = nullptr;
	FieldBoundary* boundary = nullptr;
	Object* timesup = nullptr;

	int score;
	float timer;
	FrameTimer enemy_timer;
	int polyCount;
	Vector2 boundary_margin;

	void(SceneGame::*pUpdate)(void);
	void update_fadeWait(void);
	void update_main(void);
	void update_death(void);
	void update_timeup(void);
	void swapEnemy(void);
};
