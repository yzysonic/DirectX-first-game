#pragma once
#include"Object.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitObjectManager(void);
void UpdateObjectManager(void);
void UninitObjectManager(void);

Object* ObjectManager_GetObj();
void ObjectManager_ReleaseObj(Object*& thiz);
bool ObjectManager_UpdateList_Add(Object* thiz);
void ObjectManager_UpdateList_Remove(Object* thiz);