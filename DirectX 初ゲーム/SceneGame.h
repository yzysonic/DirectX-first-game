#pragma once
#include "Core/Scene.h"
#include "Core/Core.h"
#include "NumberUI.h"
#include "SmoothCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PolygonElement.h"
#include "MiniMap.h"
#include "LiveUI.h"

#define FIELD_RANG_X (float)(SystemParameters::ResolutionX/2+500)
#define FIELD_RANG_Y (float)(SystemParameters::ResolutionY/2+500)
#define ENEMY_MAX (5)
#define GAME_POLY_MAX (1500)

class SceneGame : public Scene
{
public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;
	SmoothCamera* getCamera(void);
	int getGameScore(void);
	void addGameScore(int score);

private:
	Object2D* vignetting = nullptr;
	LiveUI* liveUI = nullptr;
	NumberUI* scoreUI = nullptr;
	NumberUI* timeUI[2] = {};
	SmoothCamera* camera = nullptr;
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
