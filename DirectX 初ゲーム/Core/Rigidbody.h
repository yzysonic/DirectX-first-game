#pragma once
#include "Component.h"
#include "Vector.h"

struct RigidbodyConstraints
{
	bool pos_x;
	bool pos_y;
	bool pos_z;
};

class Rigidbody : public Component
{
public:
	Vector3		position;
	Vector3		rotation;
	Vector3		velocity;
	Vector3		acceleration;	// ‰Á‘¬“x
	Vector3		force;			// —Í
	Vector3		net_force;		// ‡—Í
	float		mass;			// ¿—Ê
	float		bounciness;		// ’e«ŒW”
	bool useGravity;
	int listIndex;
	RigidbodyConstraints constraints;

	Rigidbody(void);
	~Rigidbody(void);
	void BindObject(Object* object) override;
	bool SetActive(bool value) override;

};