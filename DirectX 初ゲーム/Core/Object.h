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
class ObjectBase;
class RectPolygon;
class RectPolygon2D;
class Rigidbody2D;
class Collider2D;
//class Script;

class Transform
{
public:
	ObjectBase *object;
	Vector3 position;
	Vector3 scale;

	Transform(ObjectBase* object);
	Vector3 getRotation(void) const;
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

class ObjectBase
{
	friend class ObjectManager;

public:
	ObjectType type;
	int objectIndex;
	std::string name;

	ObjectBase(void);
	ObjectBase(Vector3 position, Vector3 rotation);
	virtual ~ObjectBase(void);

	virtual void update(void) {};

	Transform *	getTransform(void) const;
	//template<class T>
	//Script*			getScript(void);
	//template<class T>
	//void			setScript(void);

	void setActive(bool active);
	bool getActive(void);
	virtual void setVisibility(bool visible);

	static void* operator new(std::size_t, int _BlockUse, char const* _FileName, int _LineNumber);
	static void* operator new(std::size_t);
	static void operator delete(void*) noexcept;
	static void operator delete(void*, int _BlockUse, char const* _FileName, int _LineNumber) noexcept;

protected:
	std::unique_ptr<Transform> transform;
	//std::unordered_map<size_t, std::unique_ptr<Script>> script;

private:
	bool isActive;
	bool kill_flag;

};

class Object : public ObjectBase
{
public:
	Object(void);
	Object(Vector3 position, Vector3 rotation);
	virtual ~Object(void);
	virtual void onCollision(Object* other) {};

	RectPolygon*	getPolygon(void);
	void			setPolygon(Layer layer = Layer::DEFAULT, Texture* texName = Texture::none, RendererType rendType = RendererType::Default);

protected:
	std::unique_ptr<RectPolygon> polygon;
};

class Object2D : public ObjectBase
{
public:
	Object2D(void);
	Object2D(Vector3 position, Vector3 rotation);
	virtual ~Object2D(void);
	virtual void onCollision(Object2D* other) {};

	RectPolygon2D*	getPolygon(void);
	void			setPolygon(Layer layer = Layer::DEFAULT, Texture* texName = Texture::none, RendererType rendType = RendererType::Classic2D, std::string render_space = "default");
	Rigidbody2D*	getRigidbody(void);
	void			setRigidbody(void);
	Collider2D*		getCollider(void);
	void			setCollider(void);
	void			setVisibility(bool visible) override;

protected:
	std::unique_ptr<RectPolygon2D> polygon;
	std::unique_ptr<Rigidbody2D> rigidbody;
	std::shared_ptr<Collider2D> collider;
};