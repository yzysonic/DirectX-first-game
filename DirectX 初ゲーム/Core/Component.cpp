#include "Component.h"

Component::Component(void) : object(nullptr), active(false), active_on_load(true), type(ComponentType::Unknow) {}

void Component::BindObject(Object * object)
{
	this->object = object;
}

bool Component::SetActive(bool value)
{
	if(this->object == nullptr || this->active == value)
		return false;

	this->active = value;
	return true;
}

ComponentType Component::GetType(void)
{
	return this->type;
}

bool Component::GetActive(void)
{
	return this->active;
}
