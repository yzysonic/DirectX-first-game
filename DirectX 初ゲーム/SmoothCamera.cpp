#include "SmoothCamera.h"

SmoothCamera::SmoothCamera(Transform * target, Vector3 pos) : Camera(pos)
{

	this->target = target;
	this->speed = CAMERA_SPEED;
	this->transform->position.z = -5.0f;

}

void SmoothCamera::update()
{
	this->transform->position = Vector3::Lerp(this->transform->position, this->target->position + Vector3(0, 0, -1.0f), speed*Time::DeltaTime());
}