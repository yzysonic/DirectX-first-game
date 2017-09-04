#pragma once
#include "main.h"

typedef struct _Object Object;

typedef struct _RigidbodyConstraints
{
	bool pos_x;
	bool pos_y;
	bool pos_z;
}RigidbodyConstraints;

typedef struct _Rigidbody
{
	Object *object;
	Vector3 position;
	Vector3 rotation;
	Vector3 velocity;
	Vector3 force;
	float mass;
	bool useGravity;
	int listIndex;
	RigidbodyConstraints constraints;
}Rigidbody;

Rigidbody* newRigidbody(Object* object);
void deleteRigidbody(Rigidbody* thiz);