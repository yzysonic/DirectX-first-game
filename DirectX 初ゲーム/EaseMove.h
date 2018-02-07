#pragma once
#include "Core\Core.h"

class EaseMove : public Script
{
public:
	EaseMove(void);
	void Update(void) override;
	void SetInterval(float value);
	void MoveTo(Vector3 pos);
	void MoveTo(float x, float y, float z);

private:
	FrameTimer timer;
	Vector3 last_pos;
	Vector3 target_pos;
	float interval = 0.5f;

};