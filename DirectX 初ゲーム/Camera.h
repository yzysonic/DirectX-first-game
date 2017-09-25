#pragma once
#include "Vector.h"
#include "Object.h"

class Camera : public Object
{
public:
	Color backColor;
	float fov;

	Camera(Vector3 pos = Vector3(0, 0, -1.0f));
};