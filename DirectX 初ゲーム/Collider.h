#pragma once
#include "main.h"

typedef struct _Object Object;

typedef struct _Collider
{
	Object* object;
	bool isTrigger;
	Vector3 offset;
	Vector3 size;
	int listIndex;
}Collider;

Collider* newCollider(Object* object);
void deleteCollider(Collider* thiz);