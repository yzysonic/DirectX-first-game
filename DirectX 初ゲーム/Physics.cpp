#include "Physics.h"
#include "main.h"
#include "Object.h"
#include "Time.h"

Collider *g_ColliderList[ObjectMax];
Rigidbody *g_RigidbodyList[ObjectMax];
int g_ColliderListTop = -1;
int g_RigidbodyListTop = -1;
Vector3 g_Gravaty = Vector3(0, 10, 0);

void UpdateDynamics();
void TestCollisions();

void UpdatePhysics()
{
	UpdateDynamics();
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

void Physics_AddRigidbody(Rigidbody * thiz)
{
	if ((g_RigidbodyListTop + 1) < ObjectMax)
	{
		g_RigidbodyList[++g_RigidbodyListTop] = thiz;
	}
}

void Physics_RemoveRigidbody(Rigidbody * thiz)
{
	g_RigidbodyList[thiz->listIndex] = g_RigidbodyList[g_RigidbodyListTop--];
	thiz->listIndex = -1;
}

void Physics_SetGravity(Vector3 value)
{
	g_Gravaty = value;
}


void UpdateDynamics()
{
	Rigidbody *rb;
	float dt = GetDeltaTime();

	for (int i = 0; i <= g_RigidbodyListTop; i++)
	{

		rb = g_RigidbodyList[i];
		Vector3 a = rb->force / rb->mass;

		//x���̏���
		if (!rb->constraints.pos_x)
		{
			rb->position.x += rb->velocity.x*dt + 0.5f*a.x*dt*dt;			//�ʒu�̍X�V
			rb->velocity.x += 0.5f*(a.x + rb->useGravity*g_Gravaty.x)*dt;	//���x�̍X�V(1)
																			//�͂̌v�Z
			rb->velocity.x += 0.5f*(a.x + rb->useGravity*g_Gravaty.x)*dt;	//���x�̍X�V(2)
		}

		//y���̏���
		if (!rb->constraints.pos_y)
		{
			//x���̏����Ɠ�����
			rb->position.y += rb->velocity.y*dt + 0.5f*a.y*dt*dt;			
			rb->velocity.y += 0.5f*(a.y + rb->useGravity*g_Gravaty.y)*dt;	
			rb->velocity.y += 0.5f*(a.y + rb->useGravity*g_Gravaty.y)*dt;	
		}

		//z���̏���
		if (!rb->constraints.pos_z)
		{
			//x���̏����Ɠ�����
			rb->position.z += rb->velocity.z*dt + 0.5f*a.z*dt*dt;			
			rb->velocity.z += 0.5f*(a.z + rb->useGravity*g_Gravaty.z)*dt;	
			rb->velocity.z += 0.5f*(a.z + rb->useGravity*g_Gravaty.z)*dt;	
		}


	}

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
