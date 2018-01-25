#include "Object.h"
#include "ObjectManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

Object::Object()
{
	this->transform.object = this;
	this->type			= ObjectType::Object;
	this->isActive		= true;
	this->name			= "Object";
	this->group			= 0;
	this->kill_flag		= false;
	this->SetActive(true);
}

Object::Object(Vector3 position, Vector3 rotation) : Object()
{
	this->transform.position = position;
	this->transform.setRotation(rotation);
}

Object::~Object()
{

	this->SetActive(false);
	if (this->components.size() > 0)
	{
		for (auto &component : this->components)
			component.second.reset();
	}

}

void Object::SetActive(bool value)
{
	if (this->isActive == value)
		return;

	this->isActive = value;

	if (value)
	{
		int i = 0;
		for (auto &component : this->components)
			component.second->SetActive(this->component_actives[i++]);
		this->component_actives.clear();
	}
	else
	{
		for (auto &component : this->components)
		{
			this->component_actives.push_back(component.second->active);
			component.second->SetActive(false);
		}
	}
	
}

bool Object::GetActive(void)
{
	return this->isActive;
}

void Object::Destroy(void)
{
	if (this->kill_flag == false)
		ObjectManager::GetInstance()->AddKill(this);
}


void * Object::operator new(std::size_t size)
{
	return ObjectManager::GetInstance()->NewObject(size);
}

void Object::operator delete(void * ptr) noexcept
{

	if (ptr == nullptr)
		return;

	ObjectManager::GetInstance()->DeleteObject((Object*)(ptr));
}

#ifdef _DEBUG

void * Object::operator new(std::size_t size, int _BlockUse, char const* _FileName, int _LineNumber)
{
	return ObjectManager::GetInstance()->NewObject(size, _BlockUse, _FileName, _LineNumber);
}

void Object::operator delete(void * ptr, int _BlockUse, char const * _FileName, int _LineNumber) noexcept
{
	delete (Object*)ptr;
}

#endif