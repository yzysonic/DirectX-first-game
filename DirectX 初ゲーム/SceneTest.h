#pragma once

#include "Core\Core.h"
#include "Core/Scene.h"
#include "SmoothCamera.h"
#include "PolygonElement.h"
#include "Player.h"
#include "Enemy.h"
#include "MiniMap.h"

class SceneTest : public Scene
{
private:
	constexpr static int PolyMax = 1500;
	constexpr static float FieldRangeX = 1280.0f/2.0f+500.0f;
	constexpr static float FieldRangeY = 720.0f/2.0f+500.0f;

public:
	void init(void) override;
	void update(void) override;
	void uninit(void) override;

	SmoothCamera* camera = nullptr;
	MiniMap* mini_map = nullptr;
	PolygonElement *polyList[PolyMax] = {};
	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Object* test = nullptr;

	int polyCount;

};