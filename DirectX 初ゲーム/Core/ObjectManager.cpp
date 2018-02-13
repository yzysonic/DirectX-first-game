#include "ObjectManager.h"
#include "GameManager.h"
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
	KillObject();
}

void ObjectManager::KillObject(void)
{
	for (auto object : m_pInstance->killList)
		delete object;

	m_pInstance->killList.clear();
}

void * ObjectManager::NewObject(std::size_t size)
{
	Object* new_obj = (Object*)malloc(size);
	if (new_obj == 0)
		return nullptr;

	//InitObject(new_obj);

	return new_obj;
}

void * ObjectManager::NewObjectArray(std::size_t size)
{
	Object* new_obj = (Object*)malloc(size);

	if (new_obj == 0)
		return nullptr;

	//UINT obj_num = size / sizeof(Object);
	//for (UINT i = 0; i < obj_num; i++)
	//	InitObject(new_obj + i);

	return new_obj;
}

#ifdef _DEBUG
void * ObjectManager::NewObject(std::size_t size, int _BlockUse, char const * _FileName, int _LineNumber)
{
	Object *new_obj = (Object*)_malloc_dbg(size, _BlockUse, _FileName, _LineNumber);

	if (new_obj == 0)
		return nullptr;

	//InitObject(new_obj);

	return new_obj;
}
void * ObjectManager::NewObjectArray(std::size_t size, int _BlockUse, char const * _FileName, int _LineNumber)
{
	Object *new_obj = (Object*)_malloc_dbg(size, _BlockUse, _FileName, _LineNumber);

	if (new_obj == 0)
		return nullptr;

	//UINT obj_num = size / sizeof(Object);
	//for (UINT i = 0; i < obj_num; i++)
	//	InitObject(new_obj + i);

	return new_obj;
}
#endif

void ObjectManager::InitObject(Object * object)
{
	this->objectList.emplace_back();
	this->objectList.back().reset(object);
	this->objectList.back()->objectIndex = this->objectList.size() - 1;

	Scene* scene = GameManager::GetInstance()->GetScene();
	if (scene == nullptr)
		scene = GameManager::GetInstance()->GetGlobalScene();
	if(scene != nullptr)
		scene->AddObject(object);
}

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