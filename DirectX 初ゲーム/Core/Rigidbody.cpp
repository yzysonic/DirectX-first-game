#include "Rigidbody.h"
#include "Object.h"
#include "Physics.h"

Rigidbody2D::Rigidbody2D(ObjectBase * object)
{

	this->object = object;
	this->force = Vector3(0, 0, 0);
	this->position = Vector3(0, 0, 0);
	this->velocity = Vector3(0, 0, 0);
	this->rotation = Vector3(0, 0, 0);
	this->mass = 1;
	this->useGravity = true;
	this->constraints.pos_x = false;
	this->constraints.pos_y = false;
	this->constraints.pos_z = false;
	this->listIndex = -1;

}

Rigidbody2D::~Rigidbody2D()
{
	Physics::GetInstance()->removeRigidbody(this);
}
