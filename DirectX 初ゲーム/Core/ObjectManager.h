#pragma once

#include "Object.h"
#include "Singleton.h"
#include <vector>

class ObjectManager : public Singleton<ObjectManager>
{
public:
	static void Create(void);
	static void Destroy(void);
	static void Update(void);
	void addUpdate(ObjectBase* obj);
	void removeUpdate(ObjectBase* obj);

private:
	std::vector<ObjectBase*> updateList;
};