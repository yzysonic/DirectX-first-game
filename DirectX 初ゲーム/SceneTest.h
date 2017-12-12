#pragma once

#include "Core\Core.h"
#include "Core/Scene.h"
#include "SmoothCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "MiniMap.h"

class SceneTest : public Scene
{
public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;

	SmoothCamera* camera = nullptr;
	MiniMap* mini_map = nullptr;
	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Object* test = nullptr;

	int testCount;
};