#pragma once

typedef struct _Object Object;

enum ObjectType
{
	Obj_Normal,
	Obj_PolygonElement,
	Obj_Camera,
	Obj_Player,
	Obj_Bullet,
	Obj_Enemy,
	Obj_Test,
	ObjTypeMax
};

typedef struct _ObjectTypeFunc
{
	void(*init)(Object* thiz);
	void(*update)(Object* thiz);
	void(*uninit)(Object* thiz);
	void(*onCollision)(Object* thiz, Object* other);
}ObjectTypeFunc;


void InitObjectType(void);
ObjectTypeFunc* GetObjectTypeFunc(ObjectType type);