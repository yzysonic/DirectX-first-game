#pragma once
#include "Core.h"

#define CAMERA_SPEED (5.0f);

class SmoothCamera : public Camera
{
public:
	Transform *target;
	float speed;

	SmoothCamera(Transform* target, Vector3 pos = Vector3(0, 0, -1.0f));
	void update(void) override;
};