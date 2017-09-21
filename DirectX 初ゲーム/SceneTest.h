#pragma once

#include"Scene.h"
#include "SmoothCamera.h"
#include "Player.h"

class SceneTest : public Scene
{
public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;

	SmoothCamera* camera = nullptr;
	Player* player = nullptr;
	int testCount;
};