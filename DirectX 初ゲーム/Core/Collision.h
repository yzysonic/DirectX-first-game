#pragma once

#include "Common.h"
#include "Collider.h"
#include "Rigidbody.h"

struct Collision
{
	Vector3 point;
	Vector3 normal;
	Vector3 offset;
};

void CollisionInit(void);
bool CollisionTestAABB2D(Collider* collider_a, Collider* collider_b, Collision* pOutCollision);
bool CollisionTestSphere(Collider* collider_a, Collider* collider_b, Collision* pOutCollision);
bool CollisionTest(Collider* collider_a, Collider* collider_b, Collision* pOutCollision);
void CollisionResponse(const Collision *collision, Rigidbody *rb);
