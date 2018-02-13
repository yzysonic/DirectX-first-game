#include "CameraFov.h"

CameraFov::CameraFov(void)
{
	active_on_load = false;
}

void CameraFov::Init(void)
{
	last_fov = camera->fov;	

	float distance = fabsf(camera->transform.position.z - camera->at.z);
	near_z_offset = distance - camera->near_z;
	far_z_offset = camera->far_z - distance;

	timer.Reset();
}

void CameraFov::Update(void)
{
	SetFov(Lerpf(camera->fov, target_fov, timer.Elapsed()*speed));
	if (fabsf(camera->fov - target_fov) <= 0.00001f)
	{
		SetFov(target_fov);
		SetActive(false);
	}
	timer++;
}

void CameraFov::BindObject(Object * object)
{
	Component::BindObject(object);
	camera = dynamic_cast<Camera*>(this->object);
	Init();
}

void CameraFov::SetFov(float fov, float speed)
{
	if (speed == 0.0f)
	{
		float distance = 0.5f*SystemParameters::ResolutionY / tanf(0.5f*fov);
		camera->fov = 2.0f*atan2f(0.5f*SystemParameters::ResolutionY, distance);
		camera->transform.position.z = this->camera->at.z - distance;
		camera->near_z = distance - near_z_offset;
		camera->far_z = distance + far_z_offset;
	}
	else
	{
		this->target_fov = fov;
		this->speed = speed;
		SetActive(true);
	}
}
