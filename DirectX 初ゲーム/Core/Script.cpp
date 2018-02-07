#include "Script.h"

Script::Script(void)
{
	this->type = ComponentType::Script;
}

bool Script::SetActive(bool value)
{
	this->Component::SetActive(value);

	if (value) 
		Init();
	else 
		Uninit();

	return true;
}
