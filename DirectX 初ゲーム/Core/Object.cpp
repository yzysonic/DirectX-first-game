#include "Object.h"
#include "ObjectManager.h"
#include "Polygon.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Script.h"
#include "Physics.h"
#include "Lerp.h"
#include "Time.h"

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
	this->up			= Vector3(0, 1, 0);

}

Vector3 Transform::getRotation(void)
{
	return this->rotation;
}

void Transform::setRotation(Vector3 rotation)
{
	this->rotation = rotation;
	this->updateVector();
}

void Transform::setRotation(float x, float y, float z)
{
	this->rotation.x = x;
	this->rotation.y = y;
	this->rotation.z = z;
	this->updateVector();

}

Vector3 Transform::getUp(void)
{
	return this->up;
}

void Transform::setUp(Vector3 up)
{
	this->up = up.normalized();
	this->rotation.z = atan2f(up.y, up.x) - PI / 2;
}

void Transform::rotate(Vector3 angle)
{
	this->rotation += angle;
	this->updateVector();
}

void Transform::rotate(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
	if (rotation.x > 2 * PI)
		rotation.x -= (int)(rotation.x / 2 / PI) * 2 * PI;
	if (rotation.y > 2 * PI)
		rotation.y -= (int)(rotation.y / 2 / PI) * 2 * PI;
	if (rotation.z > 2 * PI)
		rotation.z -= (int)(rotation.z / 2 / PI) * 2 * PI;
	this->updateVector();
}

void Transform::lookAt(Vector3 const & target)
{
	Vector3 dis = target - this->position;

	this->rotation.z = atan2f(dis.y, dis.x) - PI / 2.0f;
	this->updateVector();
}

void Transform::lookAt(Transform * target)
{
	lookAt(target->position);
}

void Transform::updateVector(void)
{
	this->up.x = cosf(rotation.z + PI / 2);
	this->up.y = sinf(rotation.z + PI / 2);
}



Object::Object()
{

	this->transform		= std::make_unique<Transform>(this);
	this->type			= ObjectType::Object;
	this->isActive		= false;
	this->updateIndex	= -1;
	this->name			= "Object";
	this->setActive(true);
}

Object::Object(Vector3 position, Vector3 rotation) : Object()
{
	this->transform->position = position;
	this->transform->setRotation(rotation);
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
	this->polygon = std::make_unique<RectPolygon>(this, layer, texName, rendType);
}

Rigidbody * Object::getRigidbody(void)
{
	return this->rigidbody.get();
}

void Object::setRigidbody(void)
{
	this->rigidbody.reset();
	this->rigidbody = std::make_unique<Rigidbody>(this);
	this->rigidbody->position = this->transform->position;
	this->rigidbody->rotation = this->transform->getRotation();
	Physics::GetInstance()->addRigidbody(this->rigidbody.get());
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

template<class T>
Script * Object::getScript(void)
{
	return this->script[typeid(T).hash_code()].get();
}

template<class T>
void Object::setScript(void)
{
	this->script[typeid(T).hash_code()].reset(new T);
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
