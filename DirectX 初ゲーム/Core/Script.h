#pragma once

class Object;
class Transform;

class Script
{
public:
	Object& object;
	Transform& transform;
	Script(Object& object);
	virtual void Update(void) {};
};