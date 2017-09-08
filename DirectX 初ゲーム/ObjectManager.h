#pragma once
#include"Object.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitObjectManager(void);
void UpdateObjectManager(void);
void UninitObjectManager(void);

Object* ObjectManager_GetObj();
void ObjectManager_ReleaseObj(Object*& thiz);
bool ObjectManager_UpdateList_Add(Object* thiz);
void ObjectManager_UpdateList_Remove(Object* thiz);