#pragma once

#include "main.h"
#include "ObjectType.h"
#include "Layer.h"
#include "Texture.h"
#include "RendererType.h"

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
	Vector3 rotation;
	Vector3 scale;

	Transform(Object* object);
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