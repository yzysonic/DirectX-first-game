#include "Collision.h"
#include<functional>

std::function<bool(Collider*, Collider*, Collision*)> collision_test[(int)ColliderType::Max][(int)ColliderType::Max];

void CollisionInit(void)
{
	collision_test[(int)ColliderType::BoxCollider2D][(int)ColliderType::BoxCollider2D] = CollisionTestAABB2D;
	collision_test[(int)ColliderType::SphereCollider][(int)ColliderType::SphereCollider] = CollisionTestSphere;
}

bool CollisionTestAABB2D(Collider * collider_a, Collider * collider_b, Collision * pOutCollision)
{
	const BoxCollider2D* bca = dynamic_cast<BoxCollider2D*>(collider_a);
	const BoxCollider2D* bcb = dynamic_cast<BoxCollider2D*>(collider_b);
	const Vector3 &posa = collider_a->object->transform.position;
	const Vector3 &posb = collider_b->object->transform.position;

	return	((posa.x - bca->size.x / 2) < (posb.x + bcb->size.x / 2)) &&
			((posa.x + bca->size.x / 2) > (posb.x - bcb->size.x / 2)) &&
			((posa.y - bca->size.y / 2) < (posb.y + bcb->size.y / 2)) &&
			((posa.y + bca->size.y / 2) > (posb.y - bcb->size.y / 2));
}

bool CollisionTestSphere(Collider* collider_a, Collider* collider_b, Collision * pOutCollision)
{
	const SphereCollider* sca = dynamic_cast<SphereCollider*>(collider_a);
	const SphereCollider* scb = dynamic_cast<SphereCollider*>(collider_b);
	const Vector3 &posa = collider_a->object->transform.position;
	const Vector3 &posb = collider_b->object->transform.position;

	float distance = (posa - posb).sqrLength();
	if (distance > (sca->radius + scb->radius) * (sca->radius + scb->radius))
		return false;

	if (pOutCollision)
	{
		pOutCollision->normal = (posa - posb).normalized();
		pOutCollision->offset = (sca->radius + scb->radius - distance) * pOutCollision->normal;
	}

	return true;
}

bool CollisionTest(Collider * collider_a, Collider * collider_b, Collision * pOutCollision)
{
	return collision_test[(int)collider_a->GetType()][(int)collider_b->GetType()](collider_a, collider_b, pOutCollision);
}


void CollisionResponse(const Collision *collision, Rigidbody *rb)
{
	Vector3 move = -collision->offset;
	move = move - Vector3::Dot(move, collision->normal)*collision->normal;
	rb->position += collision->offset + move + 0.001f*collision->normal;
	rb->object->transform.position = rb->object->transform.position;
	auto t = rb->velocity + (1.0f + rb->bounciness)*Vector3::Dot(-rb->velocity, collision->normal)*collision->normal;
	rb->velocity = rb->velocity + (1.0f + rb->bounciness)*Vector3::Dot(-rb->velocity, collision->normal)*collision->normal;

	float dot_nor_force = Vector3::Dot(rb->net_force, collision->normal);
	//if (dot_nor_force < 0)
	//	rb->force += -dot_nor_force*collision->normal;
}