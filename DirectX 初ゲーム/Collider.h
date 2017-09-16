#pragma once
#include "main.h"

class Object;

class Collider
{
public:
	Object* object;
	bool isTrigger;
	Vector3 offset;
	Vector3 size;
	int listIndex;

	Collider(Object* object);
	~Collider(void);
};
