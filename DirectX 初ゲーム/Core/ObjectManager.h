#pragma once

#include "Object.h"
#include "Singleton.h"
#include <vector>
#include <memory>

class ObjectManager : public Singleton<ObjectManager>
{
	friend class Object;
public:
	static void Create(void);
	static void Destroy(void);
	static void Update(void);

private:
	std::vector<std::unique_ptr<Object>> objectList;
	std::vector<Object*> killList;

	void* NewObject(std::size_t size);
#ifdef _DEBUG
	void* NewObject(std::size_t size, int _BlockUse, char const* _FileName, int _LineNumber);
#endif
	void AddKill(Object* obj);
	void DeleteObject(Object* obj);

};