#pragma once
#include "Collider.h"
#include "Rigidbody.h"
#include "Singleton.h"
#include <vector>
#include <memory>

#define Gravity (10.0f);

class Physics : public Singleton<Physics>
{
public:
	static void Create(void);
	static void Destroy(void);
	static void Update(void);
	void addCollider(Collider* collider);
	void removeCollider(Collider* collider);
	void addRigidbody(Rigidbody* rigidbody);
	void removeRigidbody(Rigidbody* rigidbody);
	void setGravity(Vector3 value);

private:
	Vector3 gravaty;
	std::vector<Collider*> colliderList;
	std::vector<Rigidbody*> rigidbodyList;

	void updateDynamics();
	void testCollisions();
};
