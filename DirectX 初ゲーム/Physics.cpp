#include "Physics.h"
#include "main.h"
#include "Object.h"

Collider *g_ColliderList[ObjectMax];
int g_ColliderListTop = -1;

void TestCollisions();

void UpdatePhysics()
{
	TestCollisions();
}

void Physics_AddCollider(Collider * thiz)
{
	if ((g_ColliderListTop + 1) < ObjectMax)
	{
		g_ColliderList[++g_ColliderListTop] = thiz;
	}
}

void Physics_RemoveCollider(Collider * thiz)
{
	g_ColliderList[thiz->listIndex] = g_ColliderList[g_ColliderListTop--];
	thiz->listIndex = -1;
}


void TestCollisions()
{
	Collider *a, *b;
	Vector3 posa, posb;

	for (int i = 0; i <= g_ColliderListTop - 1; i++)
	{
		for (int j = i + 1; j <= g_ColliderListTop; j++)
		{
			a = g_ColliderList[i];
			b = g_ColliderList[j];
			posa = a->object->transform->position + a->offset;
			posb = b->object->transform->position + b->offset;

			if (
				((posa.x - a->size.x / 2)<(posb.x + b->size.x / 2)) &&
				((posa.x + a->size.x / 2)>(posb.x - b->size.x / 2)) &&
				((posa.y - a->size.y / 2)<(posb.y + b->size.y / 2)) &&
				((posa.y + a->size.y / 2)>(posb.y - b->size.y / 2))
				)
			{
				a->object->onCollision(a->object, b->object);
				b->object->onCollision(b->object, b->object);
			}

		}
	}
}
