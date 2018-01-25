#include "CameraSmooth.h"

void CameraSmooth::Init(void)
{
	camera = dynamic_cast<Camera*>(object);
	distance = (object->transform.position - target->position).length();
}

void CameraSmooth::Update(void)
{
	camera->transform.position = Vector3::Lerp(camera->transform.position, target->position - Vector3(0.0f, 0.0f, distance), speed*Time::DeltaTime());
	camera->at = camera->transform.position + Vector3(0.0f, 0.0f, distance);
}