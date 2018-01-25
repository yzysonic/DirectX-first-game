#include "Physics.h"
#include "Common.h"
#include "Object.h"
#include "Time.h"
#include "Collision.h"

using namespace std;

void Physics::Create(void)
{
	Singleton::Create();

	m_pInstance->gravaty = Vector3(0, 10, 0);
	m_pInstance->colliderList.reserve(SystemParameters::ObjectMax);
	m_pInstance->rigidbodyList.reserve(SystemParameters::ObjectMax);

	CollisionInit();
}

void Physics::Destroy(void)
{
	m_pInstance->colliderList.clear();
	m_pInstance->rigidbodyList.clear();

	Singleton::Destroy();
}

void Physics::Update()
{
	m_pInstance->updateDynamics();
	m_pInstance->testCollisions();
}

void Physics::addCollider(Collider* collider)
{
	this->colliderList.push_back(collider);
	collider->listIndex = this->colliderList.size() - 1;
}

void Physics::removeCollider(Collider * collider)
{
	size_t index = collider->listIndex;

	if (index < this->colliderList.size() - 1)
	{
		this->colliderList[index] = this->colliderList.back();
		this->colliderList[index]->listIndex = index;
	}
	this->colliderList.pop_back();

	collider->listIndex = -1;

}

void Physics::addRigidbody(Rigidbody* rigidbody)
{
	this->rigidbodyList.push_back(rigidbody);
	rigidbody->listIndex = this->rigidbodyList.size() - 1;
}

void Physics::removeRigidbody(Rigidbody * rigidbody)
{

	size_t index = rigidbody->listIndex;

	if (index < this->rigidbodyList.size() - 1)
	{
		this->rigidbodyList[index] = this->rigidbodyList.back();
		this->rigidbodyList[index]->listIndex = index;
	}
	this->rigidbodyList.pop_back();

	rigidbody->listIndex = -1;
}

void Physics::setGravity(Vector3 value)
{
	this->gravaty = value;
}


void Physics::updateDynamics()
{
	float dt = Time::DeltaTime();

	for(auto rb : this->rigidbodyList)
	{

		Vector3 a = rb->force / rb->mass;

		// 合力の計算
		rb->net_force = rb->force + rb->useGravity*this->gravaty*rb->mass;

		// 物理計算
		rb->position		+= rb->velocity*dt + 0.5f*rb->acceleration*dt*dt;	//位置の更新
		rb->velocity		+= 0.5f*rb->acceleration*dt;						//速度の更新(1)
		rb->acceleration	= rb->net_force / rb->mass;							//力の計算
		rb->velocity		+= 0.5f*rb->acceleration*dt;						//速度の更新(2)

		// 持ち主オブジェクトへ反映
		rb->object->transform.position = rb->position;
		rb->object->transform.setRotation(rb->rotation);

	}

}

void Physics::testCollisions()
{

	for (size_t i = 0; i < this->colliderList.size(); i++)
	{
		for (size_t j = i + 1; j < this->colliderList.size(); j++)
		{
			Collider *a, *b;
			Collision collision;

			a = this->colliderList[i];
			b = this->colliderList[j];

			if (a->object->type == b->object->type)
				continue;

			if (CollisionTest(a, b, &collision))
			{
				a->object->OnCollision(b->object);
				b->object->OnCollision(a->object);
			}
			
		}
	}
}
