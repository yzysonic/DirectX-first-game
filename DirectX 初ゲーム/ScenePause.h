#pragma once
#include "Core/Core.h"

class PauseMenu : public Object
{
public:
	void OnDraw(void) override;
	void AfterDraw(void) override;

	PauseMenu(void);

private:
	PixelShader* shader;
};

class ScenePause : public Scene
{
public:
	void Init(void) override;
	void Update(void) override;

private:
	PauseMenu* menu;
	FrameTimer timer;
	bool exit;
};