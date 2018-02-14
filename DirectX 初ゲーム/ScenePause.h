#pragma once
#include "Core/Core.h"

class PauseMenu : public Object
{
private:
	static constexpr float ItemOffset = 10.0f;
	static constexpr float ItemMargin = 85.0f;

public:
	Object* title;
	Object* cursor;
	Object* item0;
	Object* item1;

	PauseMenu(void);
	void Update(void) override;
	int GetCursor(void);

private:
	int cursor_index;

	void SetCursor(int index);
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