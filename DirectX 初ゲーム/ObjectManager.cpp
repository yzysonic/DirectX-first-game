#include "ObjectManager.h"
#include "main.h"
#include "Player.h"
#include "test.h"

//*****************************************************************************
// ÉOÉçÅ[ÉoÉãïœêî:
//*****************************************************************************
Object g_ObjectPool[ObjectMax];
Object *g_ObjectUpdateList[ObjectMax];
int g_ObjectPoolNextObj;
int g_ObjectUpdateListTop;
ObjectTypeFunc g_ObjectTypeFuncList[ObjTypeMax];

void EmptyFunc(Object *obj) {}

//public

void InitObjectManager(void)
{
	ZeroMemory(&g_ObjectPool, sizeof(Object)*ObjectMax);
	ZeroMemory(&g_ObjectUpdateList, sizeof(Object*)*ObjectMax);
	g_ObjectPoolNextObj = 0;
	g_ObjectUpdateListTop = -1;

	for (int i = 0; i < ObjectMax; i++)
	{
		g_ObjectPool[i].poolIndex = i;
	}

	g_ObjectTypeFuncList[ObjType_Normal].init	= &EmptyFunc;
	g_ObjectTypeFuncList[ObjType_Normal].update = &EmptyFunc;
	g_ObjectTypeFuncList[ObjType_Normal].uninit = &EmptyFunc;

	g_ObjectTypeFuncList[ObjType_Player].init	= &initPlayer;
	g_ObjectTypeFuncList[ObjType_Player].update = &updatePlayer;
	g_ObjectTypeFuncList[ObjType_Player].uninit = &uninitPlayer;

	g_ObjectTypeFuncList[ObjType_Test].init	= &initTest;
	g_ObjectTypeFuncList[ObjType_Test].update = &updateTest;
	g_ObjectTypeFuncList[ObjType_Test].uninit = &uninitTest;

}

void UpdateObjectManager(void)
{
	Object *obj;
	for (int i = 0; i <= g_ObjectUpdateListTop; i++)
	{
		obj = g_ObjectUpdateList[i];
		obj->update(obj);
	}
}

void UninitObjectManager(void)
{
	for (int i = 0; i < g_ObjectUpdateListTop; i++)
	{
		deleteObject(&g_ObjectPool[i]);
	}
}

ObjectTypeFunc * ObjectManager_GetTypeFunc(ObjectType type)
{
	return &g_ObjectTypeFuncList[type];
}

Object* ObjectManager_GetObj()
{
	Object *thiz = NULL;

	if (g_ObjectPoolNextObj < ObjectMax)
	{
		thiz = &g_ObjectPool[g_ObjectPoolNextObj++];
	}

	return thiz;
}

void ObjectManager_ReleaseObj(Object * thiz)
{
	int index = thiz->poolIndex;
	g_ObjectPool[index] = g_ObjectPool[g_ObjectPoolNextObj - 1];
	g_ObjectPool[index].poolIndex = index;
	g_ObjectPoolNextObj--;
}


//private

bool ObjectManager_UpdateList_Add(Object* thiz)
{
	if (g_ObjectUpdateListTop < ObjectMax - 1)
	{
		g_ObjectUpdateListTop++;
		g_ObjectUpdateList[g_ObjectUpdateListTop] = thiz;
		thiz->updateIndex = g_ObjectUpdateListTop;

		return true;
	}
	else
	{
		return false;
	}
}

void ObjectManager_UpdateList_Remove(Object* thiz)
{
	int &top = g_ObjectUpdateListTop;

	g_ObjectUpdateList[thiz->updateIndex] = g_ObjectUpdateList[top];
	g_ObjectUpdateList[thiz->updateIndex]->updateIndex = thiz->updateIndex;
	thiz->updateIndex = -1;
	top--;

}