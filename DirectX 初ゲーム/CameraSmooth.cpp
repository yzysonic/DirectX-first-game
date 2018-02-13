#include "CameraSmooth.h"

void CameraSmooth::Init(void)
{
	camera = dynamic_cast<Camera*>(object);
	distance = 250.0f;
	//distance = (object->transform.position - target->position).length();
}

void CameraSmooth::Update(void)
{
	if (target == nullptr)
		return;

	camera->transform.position = Vector3::Lerp(camera->transform.position, target->position - Vector3(0.0f, 0.0f, distance), speed*Time::DeltaTime());
	camera->at.x = camera->transform.position.x;
	camera->at.y = camera->transform.position.y;
	camera->at.z = target->position.z;
}