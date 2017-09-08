#include "ObjectManager.h"
#include "main.h"
#include "Polygon.h"
#include "Collider.h"
#include "Rigidbody.h"

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
Object g_ObjectPool[ObjectMax];
Object *g_ObjectUpdateList[ObjectMax];
int g_ObjectPoolNextObj;
int g_ObjectUpdateListTop;

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

	InitObjectType();

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


Object* ObjectManager_GetObj()
{
	Object *thiz = NULL;

	if (g_ObjectPoolNextObj < ObjectMax)
	{
		thiz = &g_ObjectPool[g_ObjectPoolNextObj];
		thiz->poolIndex = g_ObjectPoolNextObj;
		g_ObjectPoolNextObj++;
	}

	return thiz;
}

void ObjectManager_ReleaseObj(Object*& thiz)
{
	int index = thiz->poolIndex;
	Object *obj = &g_ObjectPool[index];

	if (obj->poolIndex < g_ObjectPoolNextObj - 1)
	{
		// 最後尾のオブジェクトをindexのところに移動
		*obj = g_ObjectPool[g_ObjectPoolNextObj - 1];
		obj->poolIndex = index;

		// アドレスが変わったため参照を更新する
		g_ObjectUpdateList[obj->updateIndex] = obj;
		if(obj->owner != NULL)
			((ObjOwner*)obj->owner)->base = obj;
		if(obj->transform)
			obj->transform->object = obj;
		if(obj->polygon)
			obj->polygon->object = obj;
		if(obj->collider)
			obj->collider->object = obj;
		if(obj->rigidbody)
			obj->rigidbody->object = obj;
	}

	// メモリをクリア
	g_ObjectPool[g_ObjectPoolNextObj - 1] = {};

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

	if (thiz->updateIndex < top)
	{
		g_ObjectUpdateList[thiz->updateIndex] = g_ObjectUpdateList[top];
		g_ObjectUpdateList[thiz->updateIndex]->updateIndex = thiz->updateIndex;
	}
	g_ObjectUpdateList[top] = NULL;
	thiz->updateIndex = -1;
	top--;

}