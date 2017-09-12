#include "Physics.h"
#include "main.h"
#include "Object.h"
#include "Time.h"
#include <list>

using namespace std;

//Collider *g_ColliderList[ObjectMax];
//Rigidbody *g_RigidbodyList[ObjectMax];
list<Collider*> g_ColliderList;
list<Collider*>::iterator cia, cib;
list<Rigidbody*> g_RigidbodyList;

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
	g_ColliderList.push_back(thiz);
	thiz->listIndex = g_ColliderList.size() - 1;

	//if ((g_ColliderListTop + 1) < ObjectMax)
	//{
	//	g_ColliderList[++g_ColliderListTop] = thiz;
	//	thiz->listIndex = g_ColliderListTop;
	//}
}

void Physics_RemoveCollider(Collider * thiz)
{

	for (auto collider = g_ColliderList.begin(); collider != g_ColliderList.end(); collider++)
	{
		if (*collider == thiz)
		{
			if (thiz == *cia || thiz == *cib)
			{
				if (thiz == *cia && thiz == *cib)
					cia = cib = g_ColliderList.erase(collider);
				else if (thiz == *cia)
					cia = g_ColliderList.erase(collider);
				else
					cib = g_ColliderList.erase(collider);
			}
			else
			{
				g_ColliderList.erase(collider);
			}

			break;
		}
	}


	//if (thiz->listIndex < g_ColliderListTop)
	//{
	//	g_ColliderList[thiz->listIndex] = g_ColliderList[g_ColliderListTop];
	//	g_ColliderList[thiz->listIndex]->listIndex = thiz->listIndex;
	//}
	//g_ColliderList[g_ColliderListTop] = NULL;
	//g_ColliderListTop--;
	thiz->listIndex = -1;

}

void Physics_AddRigidbody(Rigidbody * thiz)
{
	g_RigidbodyList.push_back(thiz);
	thiz->listIndex = g_RigidbodyList.size() - 1;

	//if ((g_RigidbodyListTop + 1) < ObjectMax)
	//{
	//	g_RigidbodyList[++g_RigidbodyListTop] = thiz;
	//	thiz->listIndex = g_RigidbodyListTop;
	//}
}

void Physics_RemoveRigidbody(Rigidbody * thiz)
{

	for (auto rb = g_RigidbodyList.begin(); rb != g_RigidbodyList.end(); rb++)
	{
		if (*rb == thiz)
		{
			g_RigidbodyList.erase(rb);
			break;
		}

	}


	//if (thiz->listIndex < g_RigidbodyListTop)
	//{
	//	g_RigidbodyList[thiz->listIndex] = g_RigidbodyList[g_RigidbodyListTop];
	//	g_RigidbodyList[thiz->listIndex]->listIndex = thiz->listIndex;
	//}
	//g_RigidbodyList[g_RigidbodyListTop] = NULL;
	//g_RigidbodyListTop--;
	thiz->listIndex = -1;

}

void Physics_SetGravity(Vector3 value)
{
	g_Gravaty = value;
}


void UpdateDynamics()
{
	//Rigidbody *rb;
	float dt = GetDeltaTime();

	//for (int i = 0; i <= g_RigidbodyListTop; i++)
	for(auto rb : g_RigidbodyList)
	{

		//rb = g_RigidbodyList[i];

		Vector3 a = rb->force / rb->mass;

		//x軸の処理
		if (!rb->constraints.pos_x)
		{
			rb->position.x += rb->velocity.x*dt + 0.5f*a.x*dt*dt;			//位置の更新
			rb->velocity.x += 0.5f*(a.x + rb->useGravity*g_Gravaty.x)*dt;	//速度の更新(1)
																			//力の計算
			rb->velocity.x += 0.5f*(a.x + rb->useGravity*g_Gravaty.x)*dt;	//速度の更新(2)
		}

		//y軸の処理
		if (!rb->constraints.pos_y)
		{
			//x軸の処理と同じく
			rb->position.y += rb->velocity.y*dt + 0.5f*a.y*dt*dt;			
			rb->velocity.y += 0.5f*(a.y + rb->useGravity*g_Gravaty.y)*dt;	
			rb->velocity.y += 0.5f*(a.y + rb->useGravity*g_Gravaty.y)*dt;	
		}

		//z軸の処理
		if (!rb->constraints.pos_z)
		{
			//x軸の処理と同じく
			rb->position.z += rb->velocity.z*dt + 0.5f*a.z*dt*dt;			
			rb->velocity.z += 0.5f*(a.z + rb->useGravity*g_Gravaty.z)*dt;	
			rb->velocity.z += 0.5f*(a.z + rb->useGravity*g_Gravaty.z)*dt;	
		}

		rb->object->transform->position = rb->position;
		rb->object->transform->rotation = rb->rotation;

	}

}

void TestCollisions()
{
	Collider *a, *b;
	Vector3 posa, posb;

	if (g_ColliderList.size() == 0)
		return;

	//for (int i = 0; i <= g_ColliderListTop - 1; i++)
	for (cia = g_ColliderList.begin(); cia != std::prev(g_ColliderList.end(), 1); cia++)
	{
		//for (int j = i + 1; j <= g_ColliderListTop; j++)
		for (cib = std::next(cia, 1); cib != g_ColliderList.end(); cib++)
		{
			//a = g_ColliderList.[i];
			//b = g_ColliderList[j];
			a = *cia;
			b = *cib;

			if (a->object->type == b->object->type)
				continue;

			posa = a->object->transform->position + a->offset;
			posb = b->object->transform->position + b->offset;

			if (
				((posa.x - a->size.x / 2) < (posb.x + b->size.x / 2)) &&
				((posa.x + a->size.x / 2) > (posb.x - b->size.x / 2)) &&
				((posa.y - a->size.y / 2) < (posb.y + b->size.y / 2)) &&
				((posa.y + a->size.y / 2) > (posb.y - b->size.y / 2))
				)
			{
				a->object->onCollision(a->object, b->object);
				if (a != *cia)
					continue;
				b->object->onCollision(b->object, a->object);
			}

			if (cib == g_ColliderList.end())
				break;
		}
		if (cia == prev(g_ColliderList.end(), 1))
			break;
	}

	cia = cib = g_ColliderList.begin();
}
