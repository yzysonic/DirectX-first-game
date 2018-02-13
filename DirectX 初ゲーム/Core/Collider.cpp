#include "Collider.h"
#include "Polygon.h"
#include "Physics.h"

Collider::Collider(void)
{
	this->Component::type = ComponentType::Collider;
	this->isTrigger = false;
	//Physics::GetInstance()->addCollider(this);
}


Collider::~Collider(void)
{
	if(this->active)
		Physics::GetInstance()->removeCollider(this);
}

bool Collider::SetActive(bool value)
{
	if (!Component::SetActive(value))
		return false;

	if (value)
		Physics::GetInstance()->addCollider(this);
	else
		Physics::GetInstance()->removeCollider(this);
}

ColliderType Collider::GetType(void)
{
	return this->type;
}


BoxCollider2D::BoxCollider2D(void)
{
	
	this->object = nullptr;
	this->type = ColliderType::BoxCollider2D;
	this->offset = Vector2::zero;
	this->size = 100.0f * Vector2::one;

}


void BoxCollider2D::BindObject(Object * object)
{
	this->object = object;
	this->offset = Vector2::zero;

	auto poly = object->GetComponent<RectPolygon2D>();
	if (poly != nullptr)
		this->size = poly->getSize();
	else
		this->size = 100.0f * Vector2::one;
}


SphereCollider::SphereCollider(void)
{
	this->type = ColliderType::SphereCollider;
	this->radius = 0.5f;
}

CapsuleCollider::CapsuleCollider(void)
{
	this->type = ColliderType::CapsuleCollider;
	this->radius = 0.5f;
	this->height = 1.0f;
	this->top = Vector3(0.0f, 1.0f, 0.0f);
}
