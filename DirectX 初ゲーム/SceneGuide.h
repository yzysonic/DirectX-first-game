#pragma once
#include "Core/Scene.h"
#include "Core/Core.h"

class SceneGuide : public Scene
{
public:
	void init(void) override;
	void update(void)override;
	void uninit(void)override;

private:
	Object2D *bk;
	int state;
	float timer;
};