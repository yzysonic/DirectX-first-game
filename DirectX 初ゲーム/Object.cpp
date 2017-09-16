#include "Object.h"
#include "ObjectManager.h"
#include "Polygon.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Physics.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


Transform::Transform(Object* object)
{

	this->object		= object;
	this->position		= Vector3(0, 0, 0);
	this->rotation		= Vector3(0, 0, 0);
	this->scale			= Vector3(1, 1, 1);

}



Object::Object()
{

	this->transform		= std::make_shared<Transform>(this);
	this->type			= ObjectType::Object;
	this->isActive		= false;
	this->updateIndex	= -1;
	this->name			= "Object";
	this->setActive(true);

}

Object::~Object()
{

	this->setActive(false);
	this->transform.reset();
	this->polygon.reset();
	this->rigidbody.reset();
	this->collider.reset();
}


Transform * Object::getTransform(void)
{
	return this->transform.get();
}

RectPolygon * Object::getPolygon(void)
{
	return this->polygon.get();
}

void Object::setPolygon(Layer layer, TextureName texName, RendererType rendType)
{
	this->polygon.reset();
	this->polygon = std::make_shared<RectPolygon>(this, layer, texName, rendType);
}

Rigidbody * Object::getRigidbody(void)
{
	return this->rigidbody.get();
}

void Object::setRigidbody(void)
{
	this->rigidbody.reset();
	this->rigidbody = std::make_shared<Rigidbody>(this);
	Physics::GetInstance()->addRigidbody(this->rigidbody);
}

Collider * Object::getCollider(void)
{
	return this->collider.get();
}

void Object::setCollider(void)
{
	this->collider.reset();
	this->collider = std::make_shared<Collider>(this);
	Physics::GetInstance()->addCollider(this->collider);
}

void Object::setActive(bool value)
{
	ObjectManager* manager = ObjectManager::GetInstance();

	if (this->isActive != value)
	{
		if (value)
		{
			manager->addUpdate(this);
			this->isActive = true;
		}
		else
		{
			manager->removeUpdate(this);
			this->isActive = false;
		}
	}
}

bool Object::getActive(void)
{
	return this->isActive;
}
