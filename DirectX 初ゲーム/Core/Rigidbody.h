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
	Vector3		acceleration;	// 加速度
	Vector3		force;			// 力
	Vector3		net_force;		// 合力
	float		mass;			// 質量
	float		bounciness;		// 弾性係数
	bool useGravity;
	int listIndex;
	RigidbodyConstraints constraints;

	Rigidbody(void);
	~Rigidbody(void);
	void BindObject(Object* object) override;
	bool SetActive(bool value) override;

};