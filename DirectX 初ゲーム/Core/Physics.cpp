#include "Physics.h"
#include "Common.h"
#include "Object.h"
#include "Time.h"

using namespace std;

void Physics::Create(void)
{
	Singleton::Create();

	m_pInstance->gravaty = Vector3(0, 10, 0);
	m_pInstance->colliderList.reserve(SystemParameters::ObjectMax);
	m_pInstance->rigidbodyList.reserve(SystemParameters::ObjectMax);
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

void Physics::addCollider(shared_ptr<Collider> collider)
{
	this->colliderList.push_back(collider);
	collider->listIndex = this->colliderList.size() - 1;
}

void Physics::removeCollider(Collider * collider)
{
	size_t index = collider->listIndex;

	if (index < this->colliderList.size() - 1)
	{
		this->colliderList[index].reset();
		this->colliderList[index] = move(this->colliderList.back());
		this->colliderList[index].lock()->listIndex = index;
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

		//x軸の処理
		if (!rb->constraints.pos_x)
		{
			rb->position.x += rb->velocity.x*dt + 0.5f*a.x*dt*dt;			//位置の更新
			rb->velocity.x += 0.5f*(a.x + rb->useGravity*this->gravaty.x)*dt;	//速度の更新(1)
																			//力の計算
			rb->velocity.x += 0.5f*(a.x + rb->useGravity*this->gravaty.x)*dt;	//速度の更新(2)
		}

		//y軸の処理
		if (!rb->constraints.pos_y)
		{
			//x軸の処理と同じく
			rb->position.y += rb->velocity.y*dt + 0.5f*a.y*dt*dt;			
			rb->velocity.y += 0.5f*(a.y + rb->useGravity*this->gravaty.y)*dt;	
			rb->velocity.y += 0.5f*(a.y + rb->useGravity*this->gravaty.y)*dt;	
		}

		//z軸の処理
		if (!rb->constraints.pos_z)
		{
			//x軸の処理と同じく
			rb->position.z += rb->velocity.z*dt + 0.5f*a.z*dt*dt;			
			rb->velocity.z += 0.5f*(a.z + rb->useGravity*this->gravaty.z)*dt;	
			rb->velocity.z += 0.5f*(a.z + rb->useGravity*this->gravaty.z)*dt;	
		}

		rb->object->getTransform()->position = rb->position;
		rb->object->getTransform()->setRotation(rb->rotation);

	}

}

void Physics::testCollisions()
{
	Vector3 posa, posb;

	for (size_t i = 0; i < this->colliderList.size(); i++)
	{
		for (size_t j = i + 1; j < this->colliderList.size(); j++)
		{
			shared_ptr<Collider> a, b;

			a = this->colliderList[i].lock();
			b = this->colliderList[j].lock();

			posa = a->object->getTransform()->position + a->offset;
			posb = b->object->getTransform()->position + b->offset;

			if (
				((posa.x - a->size.x / 2) < (posb.x + b->size.x / 2)) &&
				((posa.x + a->size.x / 2) > (posb.x - b->size.x / 2)) &&
				((posa.y - a->size.y / 2) < (posb.y + b->size.y / 2)) &&
				((posa.y + a->size.y / 2) > (posb.y - b->size.y / 2))
				)
			{
				a->object->onCollision(b->object);
				if(b.use_count() > 1)
					b->object->onCollision(a->object);
			}
		}
	}
}
