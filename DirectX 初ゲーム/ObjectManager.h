#pragma once

#include "Object.h"
#include "Singleton.h"
#include <vector>

class ObjectManager : public Singleton<ObjectManager>
{
public:
	static void Create(void);
	static void Destroy(void);
	//static Object* GetObj(void);
	//static void ReleaseObj(Object* obj);
	void update(void);
	void addUpdate(Object* obj);
	void removeUpdate(Object* obj);

private:
	std::vector<Object*> updateList;
	//Object pool[ObjectMax];
	//int activeTop;
};