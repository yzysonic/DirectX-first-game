#include "Camera.h"
#include "Renderer.h"

Camera::Camera(Vector3 pos)
{
	this->type = ObjectType::Camera;
	this->transform->position = pos;
	this->setBackColor(Color(255, 255, 255, 255));
	this->fov = 0.0f;
}

Camera::~Camera(void)
{
	Renderer * renderer = Renderer::GetInstance();
	if (renderer->getCamera() == this)
		renderer->setCamera(nullptr);
}

void Camera::setBackColor(Color color)
{
	backColor = ColorRGBA(color.r, color.g, color.b, color.a);
}

void Camera::setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	backColor = ColorRGBA(r, g, b, a);
}

D3DCOLOR Camera::getD3DBackColor(void)
{
	return backColor;
}
