#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "main.h"
#include "ObjectType.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NewSubObj(type)		(type*)(newObject(Obj_##type, New(type))->owner);
#define DeleteSubObj(ptr)	if(ptr != NULL) {deleteObject(ptr->base); SafeDelete(ptr)}
#define DeleteObj(ptr)		if(ptr != NULL) {deleteObject(ptr); ptr = NULL;}
#define SetThis(type)		type* thizz = (type*)(thiz->owner);


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct _Object Object;
typedef struct _RectPolygon RectPolygon;
typedef struct _Collider Collider;
typedef struct _Rigidbody Rigidbody;

typedef struct _Transform
{
	Object *object;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
}Transform;

typedef struct _Object
{
	void *owner;
	Transform *transform;
	RectPolygon *polygon;
	Rigidbody *rigidbody;
	Collider *collider;
	
	ObjectType type;
	int updateIndex;
	int poolIndex;
	char name[128];
	bool isActive;

	void(*init)(Object*);
	void(*update)(Object*);
	void(*uninit)(Object*);
	void(*onCollision)(Object*, Object*);
}Object;


typedef struct _ObjOwner
{
	Object *base;
}ObjOwner;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
Object* newObject(ObjectType type = Obj_Object, void *owner = NULL);
void deleteObject(Object* thiz);
void Object_SetActive(Object* thiz, bool value);


#endif