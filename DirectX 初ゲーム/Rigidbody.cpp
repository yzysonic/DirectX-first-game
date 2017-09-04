#include "Rigidbody.h"
#include "Object.h"
#include "Physics.h"

Rigidbody * newRigidbody(Object * object)
{
	Rigidbody *thiz = New(Rigidbody);

	thiz->object = object;
	thiz->useGravity = true;
	thiz->listIndex = -1;

	Physics_AddRigidbody(thiz);

	return thiz;
}

void deleteRigidbody(Rigidbody * thiz)
{
	Physics_RemoveRigidbody(thiz);
}
