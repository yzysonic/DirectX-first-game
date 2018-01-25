#pragma once
#include "Vector.h"

class Object;

class Transform
{
public:
	Object* object;
	Vector3 position;
	Vector3 scale;

	Transform(void);
	Vector3 getRotation(void) const;
	void setRotation(Vector3 rotation);
	void setRotation(float x, float y, float z);
	Vector3 getUp(void);
	void setUp(Vector3 up);
	void rotate(Vector3 angle);
	void rotate(float x, float y, float z);
	void lookAt(Vector3 const& target);
	void lookAt(Transform *target);

private:
	Vector3 rotation;
	Vector3 up;

	void updateVector(void);
};
