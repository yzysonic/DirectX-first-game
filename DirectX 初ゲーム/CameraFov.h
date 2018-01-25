#pragma once
#include "Core\Core.h"
#include "CameraSmooth.h"

class CameraFov : public Script
{
public:
	float target_fov = Deg2Rad(50.0f);
	float speed = 3.0f;

	CameraFov(void);
	void Init(void) override;
	void Update(void) override;
	void BindObject(Object* object) override;
	void SetFov(float fov);

private:
	Camera* camera;
	CameraSmooth* smooth;
	FrameTimer timer;
	float last_fov;
	float last_z;
};