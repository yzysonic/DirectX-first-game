#include "ObjectManager.h"
#include "Common.h"
#include "Script.h"

void ObjectManager::Create(void)
{
	Singleton::Create();

	m_pInstance->objectList.reserve(SystemParameters::ObjectMax);

}

void ObjectManager::Destroy(void)
{
	if (m_pInstance == nullptr)
		return;

	m_pInstance->objectList.clear();

	if (m_pInstance->killList.size() > 0)
	{
		for (auto object : m_pInstance->killList)
			m_pInstance->deleteObject(object);
		m_pInstance->killList.clear();
	}

	Singleton::Destroy();

}

void ObjectManager::Update(void)
{

	for (int i = 0; i < (int)m_pInstance->objectList.size(); i++)
	{
		auto object = m_pInstance->objectList[i].get();

		if (object->isActive)
			object->update();

		if (object->kill_flag)
			m_pInstance->killList.emplace_back(object);
	}

	for (auto object : m_pInstance->killList)
		m_pInstance->deleteObject(object);

	m_pInstance->killList.clear();

}

void * ObjectManager::newObject(std::size_t size)
{
	void *pvTemp = malloc(size);
	if (pvTemp == 0)
		return nullptr;

	this->objectList.emplace_back();
	this->objectList.back().reset((ObjectBase*)pvTemp);
	this->objectList.back()->objectIndex = this->objectList.size()-1;
	
	return pvTemp;
}

void ObjectManager::deleteObject(ObjectBase * obj)
{
	if (obj == nullptr)
		return;

	int index = obj->objectIndex;

	this->objectList[index].swap(this->objectList.back());
	this->objectList[index]->objectIndex = index;
	this->objectList.back().release();
	this->objectList.pop_back();
	
	free(obj);
}