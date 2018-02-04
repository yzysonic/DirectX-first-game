#pragma once
#include "Core/Scene.h"
#include "Core/Core.h"
#include "NumberUI.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PolygonElement.h"
#include "MiniMap.h"
#include "LiveUI.h"
#include "ScoreUI.h"

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

private:
	Object* vignetting = nullptr;
	LiveUI* liveUI = nullptr;
	ScoreUI* scoreUI = nullptr;
	NumberUI* timeUI[2] = {};
	Camera* camera;
	PolygonElement *polyList[GAME_POLY_MAX] = {};
	Player* player = nullptr;
	Enemy* enemy[ENEMY_MAX] = {};
	MiniMap* minimap = nullptr;

	int score;
	float timer;
	int polyCount;

	void(SceneGame::*pUpdate)(void);
	void update_fadeWait(void);
	void update_main(void);
	void swapEnemy(void);
};
