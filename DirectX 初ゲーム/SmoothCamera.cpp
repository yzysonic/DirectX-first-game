#include "SmoothCamera.h"

SmoothCamera::SmoothCamera(Transform * target, RenderTarget* render_target) : Camera(render_target)
{

	this->target = target;
	this->speed = kCameraSpeed;
	this->distance = 100.0f;
	this->transform->position.z = -5.0f;

	this->camera_shake.SetCamera(this);
	this->camera_shake.duration = 0.8f;
	this->camera_shake.strength = 20.f;
	this->camera_shake.speed = 70.0f;
	this->camera_shake.decay = 7.0f;

}

void SmoothCamera::update()
{
	this->transform->position = Vector3::Lerp(this->transform->position, this->target->position + Vector3(0, 0, -this->distance), speed*Time::DeltaTime());
	this->at = this->transform->position + Vector3(0.0f, 0.0f, 1.0f);
}

void SmoothCamera::Shake(void)
{
	this->camera_shake.Activate();
}