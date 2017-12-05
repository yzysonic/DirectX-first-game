#pragma once

#include "Common.h"
#include "ObjectType.h"
#include "Layer.h"
#include "Texture.h"
#include "RendererType.h"
#include "Vector.h"

#include <string>
#include <memory>
#include <unordered_map>


//*****************************************************************************
// ƒNƒ‰ƒX’è‹`
//*****************************************************************************
class Object;
class RectPolygon2D;
class Rigidbody;
class Collider2D;
class Script;

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
	void setUp(Vector3 up);
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
	friend class ObjectManager;

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
	template<class T>
	Script*			getScript(void);
	template<class T>
	void			setScript(void);

	void setActive(bool value);
	bool getActive(void);

protected:
	std::unique_ptr<Transform> transform;
	std::unordered_map<size_t, std::unique_ptr<Script>> script;

private:
	bool isActive;
};

class Object2D : public Object
{
public:
	Object2D(void);
	Object2D(Vector3 position, Vector3 rotation);
	~Object2D(void);

	RectPolygon2D*	getPolygon(void);
	void			setPolygon(Layer layer = Layer::DEFAULT, TextureName texName = TEX_NONE, RendererType rendType = RendererType::Classic2D);
	Rigidbody*		getRigidbody(void);
	void			setRigidbody(void);
	Collider2D*		getCollider(void);
	void			setCollider(void);

protected:
	std::unique_ptr<RectPolygon2D> polygon;
	std::unique_ptr<Rigidbody> rigidbody;
	std::shared_ptr<Collider2D> collider;
};