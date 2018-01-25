#pragma once
#include "Core\Core.h"

class CameraSmooth : public Script
{
public:
	CameraSmooth(Object* target) : target(&target->transform) {};
	Transform *target = nullptr;
	float speed = 5.0f;
	float distance;

	void Init(void) override;
	void Update(void) override;

private:
	Camera* camera;
};

