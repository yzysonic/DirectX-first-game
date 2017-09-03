#include "Collider.h"
#include "Object.h"
#include "Polygon.h"
#include "Physics.h"

Collider* newCollider(Object * object)
{
	Collider* thiz = New(Collider);
	
	thiz->object = object;
	thiz->isTrigger = false;
	thiz->offset = Vector3(0, 0, 0);
	if (object->polygon != NULL)
		thiz->size = Vector3(object->polygon->size.x, object->polygon->size.y, 0.0f);
	else
		thiz->size = Vector3(100, 100, 100);
	thiz->listIndex = -1;

	Physics_AddCollider(thiz);

	return thiz;
}

void deleteCollider(Collider * thiz)
{
	if (thiz == NULL) return;

	Physics_RemoveCollider(thiz);
	SafeDelete(thiz);
}

