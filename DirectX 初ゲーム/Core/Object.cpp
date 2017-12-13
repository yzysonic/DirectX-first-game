#include "Object.h"
#include "ObjectManager.h"
#include "Polygon.h"
#include "Collider.h"
#include "Rigidbody.h"
//#include "Script.h"
#include "Physics.h"
#include "Math.h"
#include "Time.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


Transform::Transform(ObjectBase* object)
{

	this->object		= object;
	this->position		= Vector3(0, 0, 0);
	this->rotation		= Vector3(0, 0, 0);
	this->scale			= Vector3(1, 1, 1);
	this->up			= Vector3(0, 1, 0);

}

Vector3 Transform::getRotation(void) const
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



ObjectBase::ObjectBase()
{

	this->transform		= std::make_unique<Transform>(this);
	this->type			= ObjectType::Object;
	this->isActive		= false;
	this->updateIndex	= -1;
	this->name			= "Object";
	this->setActive(true);
}

ObjectBase::ObjectBase(Vector3 position, Vector3 rotation) : ObjectBase()
{
	this->transform->position = position;
	this->transform->setRotation(rotation);
}

ObjectBase::~ObjectBase()
{

	this->setActive(false);
	this->transform.reset();

}


Transform * ObjectBase::getTransform(void) const
{
	return this->transform.get();
}

//template<class T>
//Script * ObjectBase::getScript(void)
//{
//	return this->script[typeid(T).hash_code()].get();
//}
//
//template<class T>
//void ObjectBase::setScript(void)
//{
//	this->script[typeid(T).hash_code()].reset(new T);
//}


void ObjectBase::setActive(bool active)
{
	if (this->isActive == active)
		return;

	if (active)
		ObjectManager::GetInstance()->addUpdate(this);
	else
		ObjectManager::GetInstance()->removeUpdate(this);

	this->isActive = active;
	setVisibility(active);
}

bool ObjectBase::getActive(void)
{
	return this->isActive;
}

void ObjectBase::setVisibility(bool visible){}

Object::Object(void) : ObjectBase() {}

Object::Object(Vector3 position, Vector3 rotation) : ObjectBase(position, rotation) {}

Object::~Object(void)
{
	this->polygon.reset();
}

RectPolygon* Object::getPolygon(void)
{
	return this->polygon.get();
}

void Object::setPolygon(Layer layer, Texture* texName, RendererType rendType)
{
	this->polygon.reset();
	this->polygon = std::make_unique<RectPolygon>(this, layer, texName, rendType);
}

Object2D::Object2D(void) : ObjectBase() {}

Object2D::Object2D(Vector3 position, Vector3 rotation) : ObjectBase(position, rotation) {}

Object2D::~Object2D(void)
{
	this->polygon.reset();
	this->rigidbody.reset();
	this->collider.reset();
}

RectPolygon2D * Object2D::getPolygon(void)
{
	return this->polygon.get();
}

void Object2D::setPolygon(Layer layer, Texture* texName, RendererType rendType, std::string render_space)
{
	this->polygon.reset();
	this->polygon = std::make_unique<RectPolygon2D>(this, layer, texName, rendType, render_space);
}

Rigidbody2D * Object2D::getRigidbody(void)
{
	return this->rigidbody.get();
}

void Object2D::setRigidbody(void)
{
	this->rigidbody.reset();
	this->rigidbody = std::make_unique<Rigidbody2D>(this);
	this->rigidbody->position = this->transform->position;
	this->rigidbody->rotation = this->transform->getRotation();
	Physics::GetInstance()->addRigidbody(this->rigidbody.get());
}

Collider2D * Object2D::getCollider(void)
{
	return this->collider.get();
}

void Object2D::setCollider(void)
{
	this->collider.reset();
	this->collider = std::make_shared<Collider2D>(this);
	Physics::GetInstance()->addCollider(this->collider);
}

void Object2D::setVisibility(bool visible)
{
	this->polygon->setVisibility(visible);
}

