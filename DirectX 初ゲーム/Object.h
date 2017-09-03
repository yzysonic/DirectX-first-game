#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include"main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define SetNewObj(ptr, type)	ptr = New(type); ptr->base = newObject(ObjType_##type, ptr);
//#define NewObj(type)		{type* ptr; SetNewObj(ptr, type)}
#define NewObj(type)	(type*)(newObject(ObjType_##type, New(type))->owner);
#define DeleteObj(ptr)	if(ptr != NULL) {deleteObject(ptr->base); SafeDelete(ptr); ptr = NULL;}
#define SetThis(type)	type* this##type = (type*)(thiz->owner);


typedef enum
{
	ObjType_Normal,
	ObjType_Camera,
	ObjType_Player,
	ObjType_Enemy,
	ObjType_Test,
	ObjTypeMax
}ObjectType;


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct _Object Object;
typedef struct _RectPolygon RectPolygon;

typedef struct _Transform
{
	Object *object;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
}Transform;


typedef struct _Rigidbody
{
	Object *object;
	Vector3 position;
	Vector3 rotation;
	Vector3 velocity;
	bool useGravity;
}Rigidbody;

typedef struct _Collider
{
	Object *object;
	bool isTrigger;
}Collider;

typedef struct _BoxCollider
{
	Collider *base;
	Vector3 cneter;
	Vector3 size;
}BoxCollider;

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
	char name[128] = "Object";
	bool isActive;

	void(*init)(Object*);
	void(*update)(Object*);
	void(*uninit)(Object*);
}Object;

typedef struct _ObjectTypeFunc
{
	void(*init)(Object*);
	void(*update)(Object*);
	void(*uninit)(Object*);
}ObjectTypeFunc;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
Object* newObject(ObjectType type = ObjType_Normal, void *owner = NULL);
void deleteObject(Object* thiz);
void Object_SetActive(Object* thiz, bool value);


#endif