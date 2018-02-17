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

#define FIELD_RANG_X (float)(SystemParameters::ResolutionX/2+500)
#define FIELD_RANG_Y (float)(SystemParameters::ResolutionY/2+500)
#define ENEMY_MAX (5)
#define GAME_POLY_MAX (1000)

class SceneGame : public Scene
{
public:
	static constexpr float GameTime = 60.0f;

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
	Enemy* enemy[ENEMY_MAX] = {};
	MiniMap* minimap = nullptr;
	FieldBoundary* boundary = nullptr;
	Object* timesup = nullptr;

	int score;
	float timer;
	FrameTimer enemy_timer;
	int polyCount;

	void(SceneGame::*pUpdate)(void);
	void update_fadeWait(void);
	void update_main(void);
	void update_death(void);
	void update_timeup(void);
	void swapEnemy(void);
};
