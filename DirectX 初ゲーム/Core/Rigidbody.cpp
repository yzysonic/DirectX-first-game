#include "Rigidbody.h"
#include "Physics.h"

Rigidbody::Rigidbody(void)
{
	this->Component::type = ComponentType::Rigidbody;

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

Rigidbody::~Rigidbody()
{
	SetActive(false);
}

void Rigidbody::BindObject(Object * object)
{
	Component::BindObject(object);
	this->position = this->object->transform.position;
	this->rotation = this->object->transform.getRotation();
}

bool Rigidbody::SetActive(bool value)
{
	if(this->Component::SetActive(value) == false)
		return false;

	if (value)
		Physics::GetInstance()->addRigidbody(this);
	else
		Physics::GetInstance()->removeRigidbody(this);

	return true;
}
