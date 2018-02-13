#pragma once
#include "Object.h"
#include "Component.h"
#include "Vector.h"

enum class ColliderType
{
	BoxCollider2D,
	SphereCollider,
	CapsuleCollider,
	Max
};

class Collider : public Component
{
public:
	bool isTrigger;
	int listIndex;

	Collider(void);
	~Collider(void);
	bool SetActive(bool value) override;
	ColliderType GetType(void);

protected:
	ColliderType type;

};

class SphereCollider : public Collider
{
public:
	float radius;

	SphereCollider(void);
};

class CapsuleCollider : public Collider
{
public:
	float radius;
	float height;
	Vector3 top;

	CapsuleCollider(void);
};

class BoxCollider2D : public Collider
{
public:
	Vector2 offset;
	Vector2 size;

	BoxCollider2D(void);
	void BindObject(Object* object) override;
};