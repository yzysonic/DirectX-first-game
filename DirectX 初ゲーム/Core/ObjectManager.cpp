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

	for (auto& object : m_pInstance->objectList)
		delete(object.release());

	for (auto object : m_pInstance->killList)
		free(object);

	Singleton::Destroy();

}

void ObjectManager::Update(void)
{

	for (int i = 0; i < (int)m_pInstance->objectList.size(); i++)
	{
		auto object = m_pInstance->objectList[i].get();

		if (object->isActive)
			object->update();

		//if (object->kill_flag)
		//	m_pInstance->killList.emplace_back(object);
	}

	for (auto object : m_pInstance->killList)
		m_pInstance->deleteObject(object);

	m_pInstance->killList.clear();

}

void * ObjectManager::newObject(std::size_t size, int _BlockUse, char const* _FileName, int _LineNumber)
{
	void *pvTemp = _malloc_dbg(size, _BlockUse, _FileName, _LineNumber);
	if (pvTemp == 0)
		return nullptr;

	this->objectList.emplace_back();
	this->objectList.back().reset((ObjectBase*)pvTemp);
	this->objectList.back()->objectIndex = this->objectList.size()-1;
	
	return pvTemp;
}

void ObjectManager::addKill(ObjectBase * obj)
{
	this->killList.emplace_back(obj);
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