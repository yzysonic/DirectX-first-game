#pragma once

class ObjectBase;
class Transform;

class Script
{
public:
	ObjectBase& object;
	Transform& transform;
	Script(ObjectBase& object);
	virtual void Update(void) {};
};