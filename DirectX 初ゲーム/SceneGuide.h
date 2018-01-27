#pragma once
#include "Core/Scene.h"
#include "Core/Core.h"

class SceneGuide : public Scene
{
public:
	void Init(void) override;
	void Update(void)override;

private:
	smart_ptr<Object> bk;
	int state;
	float timer;
};