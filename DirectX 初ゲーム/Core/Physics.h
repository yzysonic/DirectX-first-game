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
	void addCollider(std::shared_ptr<Collider2D> collider);
	void removeCollider(Collider2D* collider);
	void addRigidbody(Rigidbody2D* rigidbody);
	void removeRigidbody(Rigidbody2D* rigidbody);
	void setGravity(Vector3 value);

private:
	Vector3 gravaty;
	std::vector<std::weak_ptr<Collider2D>> colliderList;
	std::vector<Rigidbody2D*> rigidbodyList;

	void updateDynamics();
	void testCollisions();
};
