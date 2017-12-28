#include "Collider.h"
#include "Object.h"
#include "Polygon.h"
#include "Physics.h"

Collider2D::Collider2D(Object2D * object)
{
	
	this->object = object;
	this->isTrigger = false;
	this->offset = Vector3(0, 0, 0);
	if (object->getPolygon() != NULL)
		this->size = Vector3(object->getPolygon()->getSize().x, object->getPolygon()->getSize().y, 0.0f);
	else
		this->size = Vector3(100, 100, 100);
	this->listIndex = -1;

}

Collider2D::~Collider2D()
{
	Physics::GetInstance()->removeCollider(this);
}

