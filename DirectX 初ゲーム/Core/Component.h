#pragma once

#include <unordered_map>
#include <string>

class Object;

enum class ComponentType
{
	Unknow,
	RectPolygon,
	RectPolygon2D,
	Rigidbody,
	Collider,
	Collider2D,
	Script,
	Max
};

class Component
{
	friend class Object;
public:
	Object* object;
	
	Component(void);
	virtual ~Component(void) {};
	virtual void BindObject(Object* object);
	virtual bool SetActive(bool value);

	ComponentType GetType(void);
	bool GetActive(void);

protected:
	ComponentType type;
	bool active;
	bool active_on_load;
};

