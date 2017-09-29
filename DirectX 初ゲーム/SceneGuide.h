#pragma once
#include "Scene.h"
#include "Core.h"

class SceneGuide : public Scene
{
public:
	void init(void) override;
	void update(void)override;
	void uninit(void)override;

private:
	Object *bk;
	int state;
	float timer;
};