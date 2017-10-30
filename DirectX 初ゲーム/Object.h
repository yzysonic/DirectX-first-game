#pragma once

#include "Common.h"
#include "ObjectType.h"
#include "Layer.h"
#include "Texture.h"
#include "RendererType.h"
#include "Vector.h"

#include <string>
#include <memory>


//*****************************************************************************
// ƒNƒ‰ƒX’è‹`
//*****************************************************************************
class Object;
class RectPolygon;
class Rigidbody;
class Collider;

class Transform
{
public:
	Object *object;
	Vector3 position;
	Vector3 scale;

	Transform(Object* object);
	Vector3 getRotation(void);
	void setRotation(Vector3 rotation);
	void setRotation(float x, float y, float z);
	Vector3 getUp(void);
	void rotate(Vector3 angle);
	void rotate(float x, float y, float z);
	void lookAt(Vector3 const& target);
	void lookAt(Transform *target);

private:
	Vector3 rotation;
	Vector3 up;

	void updateVector(void);
};

class Object
{
public:
	ObjectType type;
	int updateIndex;
	std::string name;

	Object(void);
	Object(Vector3 position, Vector3 rotation);
	~Object(void);
	
	virtual void update(void) {};
	virtual void onCollision(Object* other) {};

	Transform*		getTransform(void);
	RectPolygon*	getPolygon(void);
	void			setPolygon(Layer layer = Layer::DEFAULT, TextureName texName = TEX_NONE, RendererType rendType = RendererType::Default);
	Rigidbody*		getRigidbody(void);
	void			setRigidbody(void);
	Collider*		getCollider(void);
	void			setCollider(void);

	void setActive(bool value);
	bool getActive(void);

protected:
	std::unique_ptr<Transform> transform;
	std::unique_ptr<RectPolygon> polygon;
	std::unique_ptr<Rigidbody> rigidbody;
	std::shared_ptr<Collider> collider;

private:
	bool isActive;
};