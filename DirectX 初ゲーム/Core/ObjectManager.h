#pragma once

#include "Object.h"
#include "Singleton.h"
#include <vector>
#include <memory>

class ObjectManager : public Singleton<ObjectManager>
{
public:
	static void Create(void);
	static void Destroy(void);
	static void Update(void);

public:
	void* newObject(std::size_t size, int _BlockUse, char const* _FileName, int _LineNumber);
	void addKill(ObjectBase* obj);
	void deleteObject(ObjectBase* obj);

private:
	std::vector<std::unique_ptr<ObjectBase>> objectList;
	std::vector<ObjectBase*> killList;
};