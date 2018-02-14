#pragma once
#include "Core\Core.h"

class MouseCursor : public Object
{
public:
	MouseCursor(void);
	void Update(void) override;

private:
	FrameTimer timer;
	int state;
};