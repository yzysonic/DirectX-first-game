#include "CameraFov.h"

CameraFov::CameraFov(void)
{
	active_on_load = false;
}

void CameraFov::Init(void)
{
	last_fov = camera->fov;
	
	if(smooth)
		last_z = smooth->target->position.z;
	else 
		last_z = 0;
	
	timer.Reset();
}

void CameraFov::Update(void)
{
	SetFov(Lerpf(camera->fov, target_fov, timer.Elapsed()*speed));
	if(camera->fov == target_fov) SetActive(false);
	timer++;
}

void CameraFov::BindObject(Object * object)
{
	Component::BindObject(object);
	camera = dynamic_cast<Camera*>(this->object);
	smooth = camera->GetComponent<CameraSmooth>();
	Init();
}

void CameraFov::SetFov(float fov)
{
	float distance = 0.5f*SystemParameters::ResolutionY / tanf(0.5f*fov);
	camera->fov = 2.0f*atan2f(0.5f*SystemParameters::ResolutionY, distance);
	camera->transform.position.z = last_z - distance;

	if (smooth) smooth->distance = distance;
}
