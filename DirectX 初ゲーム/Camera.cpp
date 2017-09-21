#include "Camera.h"

Camera::Camera(Vector3 pos)
{
	this->type = ObjectType::Camera;
	this->transform->position = pos;
	this->backColor = ColorRGBA(255, 255, 255, 255);
	this->fov = 0.0f;
}