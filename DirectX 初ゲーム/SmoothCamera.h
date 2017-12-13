#pragma once
#include "Core/Core.h"
#include "CameraShake.h"

constexpr float kCameraSpeed = 5.0f;

class SmoothCamera : public Camera
{
public:
	Transform *target;
	float speed;
	float distance;

	SmoothCamera(Transform* target, RenderTarget* render_target = nullptr);
	void update(void) override;
	void Shake(void);

private:
	CameraShake camera_shake;
};