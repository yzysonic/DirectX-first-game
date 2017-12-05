#pragma once
#include "Vector.h"

class ObjectBase;

struct RigidbodyConstraints
{
	bool pos_x;
	bool pos_y;
	bool pos_z;
};

class Rigidbody2D
{
public:
	ObjectBase *object;
	Vector3 position;
	Vector3 rotation;
	Vector3 velocity;
	Vector3 force;
	float mass;
	bool useGravity;
	int listIndex;
	RigidbodyConstraints constraints;

	Rigidbody2D(ObjectBase* object);
	~Rigidbody2D(void);
};