#pragma once
#include "Scene.h"
#include "main.h"
#include "Core.h"
#include "NumberUI.h"
#include "SmoothCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PolygonElement.h"

#define FIELD_RANG_X (SCREEN_CENTER_X+500)
#define FIELD_RANG_Y (SCREEN_CENTER_Y+500)
#define ENEMY_MAX (5)
#define GAME_POLY_MAX (1000)

class SceneGame : public Scene
{
public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;
	int getGameScore(void);
	void addGameScore(int score);

private:
	Object* vignetting = nullptr;
	Object* liveUI = nullptr;
	NumberUI* scoreUI = nullptr;
	NumberUI* timeUI[2] = {};
	SmoothCamera* camera = nullptr;
	PolygonElement *polyList[GAME_POLY_MAX] = {};
	Player* player = nullptr;
	Enemy* enemy[ENEMY_MAX] = {};

	int score;
	float timer;
	int polyCount;

	void(SceneGame::*pUpdate)(void);
	void update_fadeWait(void);
	void update_main(void);
	void swapEnemy(void);
};
