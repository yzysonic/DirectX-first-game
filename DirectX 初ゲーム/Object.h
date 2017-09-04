#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include"main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NewObj(type)	(type*)(newObject(ObjType_##type, New(type))->owner);
#define DeleteObj(ptr)	if(ptr != NULL) {deleteObject(ptr->base); SafeDelete(ptr)}
#define SetThis(type)	type* this##type = (type*)(thiz->owner);


enum ObjectType
{
	ObjType_Normal,
	ObjType_Camera,
	ObjType_Player,
	ObjType_Bullet,
	ObjType_Enemy,
	ObjType_Test,
	ObjTypeMax
};


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

typedef struct _ObjectTypeFunc
{
	void(*init)(Object* thiz);
	void(*update)(Object* thiz);
	void(*uninit)(Object* thiz);
	void(*onCollision)(Object* thiz, Object* other);
}ObjectTypeFunc;

typedef struct _ObjOwner
{
	Object *base;
}ObjOwner;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
Object* newObject(ObjectType type = ObjType_Normal, void *owner = NULL);
void deleteObject(Object* thiz);
void Object_SetActive(Object* thiz, bool value);


#endif