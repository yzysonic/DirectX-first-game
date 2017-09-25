#pragma once
#include "Vector.h"

class Object;

struct RigidbodyConstraints
{
	bool pos_x;
	bool pos_y;
	bool pos_z;
};

class Rigidbody
{
public:
	Object *object;
	Vector3 position;
	Vector3 rotation;
	Vector3 velocity;
	Vector3 force;
	float mass;
	bool useGravity;
	int listIndex;
	RigidbodyConstraints constraints;

	Rigidbody(Object* object);
	~Rigidbody(void);
};