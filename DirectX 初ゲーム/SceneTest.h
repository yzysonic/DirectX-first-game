#pragma once

#include "Core\Core.h"
#include "Core/Scene.h"
#include "PolygonElement.h"
#include "Player.h"
#include "Enemy.h"
#include "MiniMap.h"
#include "FieldPolygon.h"

class SceneTest : public Scene
{
private:
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
	Object* test = nullptr;
	FieldPolygon* field_bk = nullptr;

	int PolyMax;
	int polyCount;

};