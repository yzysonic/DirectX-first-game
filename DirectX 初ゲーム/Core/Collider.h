#pragma once
#include "Vector.h"

class Object2D;

class Collider2D
{
public:
	Object2D* object;
	bool isTrigger;
	Vector3 offset;
	Vector3 size;
	int listIndex;

	Collider2D(Object2D* object);
	~Collider2D(void);
};

