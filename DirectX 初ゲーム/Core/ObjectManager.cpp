#include "ObjectManager.h"
#include "Common.h"
#include "Script.h"

void ObjectManager::Create(void)
{
	Singleton::Create();

	m_pInstance->updateList.reserve(SystemParameters::ObjectMax);

}

void ObjectManager::Destroy(void)
{
	if (m_pInstance == nullptr)
		return;

	m_pInstance->updateList.clear();

	Singleton::Destroy();

}

void ObjectManager::Update(void)
{

	for (size_t i = 0; i < m_pInstance->updateList.size(); i++)
	{
		m_pInstance->updateList[i]->update();
	}

}

void ObjectManager::addUpdate(ObjectBase * obj)
{
	obj->updateIndex = this->updateList.size();
	this->updateList.push_back(obj);
}

void ObjectManager::removeUpdate(ObjectBase * obj)
{
	int index = obj->updateIndex;

	this->updateList[index] = this->updateList.back();
	this->updateList[index]->updateIndex = index;
	this->updateList.pop_back();

	obj->updateIndex = -1;
}
