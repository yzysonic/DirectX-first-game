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
		::delete object.release();

	Singleton::Destroy();

}

void ObjectManager::Update(void)
{

	for (int i = 0; i < (int)m_pInstance->objectList.size(); i++)
	{
		auto &object = m_pInstance->objectList[i];
		if (object->isActive)
		{
			// オブジェクトの更新処理
			object->Update();

			// オブジェクト所属のスクリプトの更新処理
			for (auto &script : object->scripts)
			{
				if (script->GetActive())
					script->Update();
			}
		}
	}

	for (auto object : m_pInstance->killList)
		delete object;

	m_pInstance->killList.clear();

}

void * ObjectManager::NewObject(std::size_t size)
{
	void *pvTemp = malloc(size);
	if (pvTemp == 0)
		return nullptr;

	this->objectList.emplace_back();
	this->objectList.back().reset((Object*)pvTemp);
	this->objectList.back()->objectIndex = this->objectList.size()-1;
	
	return pvTemp;
}

#ifdef _DEBUG
void * ObjectManager::NewObject(std::size_t size, int _BlockUse, char const * _FileName, int _LineNumber)
{
	void *pvTemp = _malloc_dbg(size, _BlockUse, _FileName, _LineNumber);

	if (pvTemp == 0)
		return nullptr;

	this->objectList.emplace_back();
	this->objectList.back().reset((Object*)pvTemp);
	this->objectList.back()->objectIndex = this->objectList.size() - 1;

	return pvTemp;
}
#endif

void ObjectManager::AddKill(Object * obj)
{
	this->killList.emplace_back(obj);
	obj->kill_flag = true;
}

void ObjectManager::DeleteObject(Object * obj)
{
	if (obj == nullptr)
		return;

	// オブジェクトリストの最後尾と交換し、削除する
	int index = obj->objectIndex;
	this->objectList[index].swap(this->objectList.back());
	this->objectList[index]->objectIndex = index;
	this->objectList.back().release();
	this->objectList.pop_back();
	
	free(obj);
}