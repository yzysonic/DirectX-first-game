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
	void update(void);
	void addCollider(std::shared_ptr<Collider> collider);
	void removeCollider(Collider* collider);
	void addRigidbody(std::shared_ptr<Rigidbody> rigidbody);
	void removeRigidbody(Rigidbody* rigidbody);
	void setGravity(Vector3 value);

private:
	Vector3 gravaty;
	std::vector<std::weak_ptr<Collider>> colliderList;
	std::vector<std::weak_ptr<Rigidbody>> rigidbodyList;

	void updateDynamics();
	void testCollisions();
};
