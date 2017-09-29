#include "Object.h"
#include "ObjectManager.h"
#include "Polygon.h"
#include "Collider.h"
#include "Rigidbody.h"
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

void Transform::rotate(Vector3 angle)
{
	this->rotation += angle;
	this->up.x = this->up.x * cosf(angle.z) - this->up.y * sinf(angle.z);
	this->up.y = this->up.x * sinf(angle.z) - this->up.y * cosf(angle.z);
	this->updateVector();
}

void Transform::rotate(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
	this->updateVector();
}

void Transform::lookAt(Vector3 const & target, float speed)
{
	Vector3 dis = target - this->position;
	float angle;

	if (dis.x > 0)
		angle = acosf(Vector3::Dot(Vector3(0, -1, 0), dis) / (dis.length()));
	else
		angle = PI + acosf(-Vector3::Dot(Vector3(0, -1, 0), dis) / (dis.length()));

	this->rotation.z = Lerpf(this->rotation.z, angle, speed * Time::DeltaTime());
}

void Transform::lookAt(Transform * target, float speed)
{
	lookAt(target->position, speed);
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
