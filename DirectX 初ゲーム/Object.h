#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include"main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

typedef enum
{
	ObjType_Normal,
	ObjType_Player,
	ObjType_Enemy,
	ObjTypeMax
}ObjectType;


//*****************************************************************************
// �\���̒�`
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
// �v���g�^�C�v�錾
//*****************************************************************************
Object* newObject(ObjectType type);
void deleteObject(Object* thiz);
void Object_SetActive(Object* thiz, bool value);


#endif