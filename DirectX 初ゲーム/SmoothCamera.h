#pragma once
#include "Core/Core.h"
#include "CameraShake.h"

constexpr float kCameraSpeed = 5.0f;

class SmoothCamera : public Camera
{
public:
	Transform *target;
	float speed;

	SmoothCamera(Transform* target, Vector3 pos = Vector3(0, 0, -1.0f));
	void update(void) override;
	void Shake(void);

private:
	CameraShake camera_shake;
};