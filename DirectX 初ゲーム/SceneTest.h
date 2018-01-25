#pragma once

#include "Core\Core.h"
#include "Core/Scene.h"
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
	void Init(void) override;
	void Update(void) override;
	void Uninit(void) override;

	Camera* camera = nullptr;
	MiniMap* mini_map = nullptr;
	std::vector<PolygonElement*> polyList;
	Player* player = nullptr;
	Enemy* enemy = nullptr;
	smart_ptr<Object> test = nullptr;

	int polyCount;

};