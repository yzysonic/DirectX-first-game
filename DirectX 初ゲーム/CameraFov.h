#pragma once
#include "Core\Core.h"

class CameraFov : public Script
{
public:
	float target_fov = Deg2Rad(50.0f);
	float speed = 3.0f;

	CameraFov(void);
	void SetFov(float fov, float speed = 0.0f);

private:
	Camera* camera;
	FrameTimer timer;
	float last_fov;
	float near_z_offset;
	float far_z_offset;

	void Init(void) override;
	void Update(void) override;
	void BindObject(Object* object) override;

};