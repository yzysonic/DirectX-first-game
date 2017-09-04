#include "Rigidbody.h"
#include "Object.h"
#include "Physics.h"

Rigidbody * newRigidbody(Object * object)
{
	Rigidbody *thiz = New(Rigidbody);

	thiz->object = object;
	thiz->force = Vector3(0, 0, 0);
	thiz->position = Vector3(0, 0, 0);
	thiz->velocity = Vector3(0, 0, 0);
	thiz->rotation = Vector3(0, 0, 0);
	thiz->mass = 1;
	thiz->useGravity = true;
	thiz->constraints.pos_x = false;
	thiz->constraints.pos_y = false;
	thiz->constraints.pos_z = false;
	thiz->listIndex = -1;

	Physics_AddRigidbody(thiz);

	return thiz;
}

void deleteRigidbody(Rigidbody * thiz)
{
	if (thiz == NULL)
		return;

	Physics_RemoveRigidbody(thiz);
	SafeDelete(thiz);
}
