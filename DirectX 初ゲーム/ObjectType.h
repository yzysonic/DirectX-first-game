#pragma once

typedef struct _Object Object;

enum ObjectType
{
	Obj_Object,
	Obj_NumberUI,
	Obj_PolygonElement,
	Obj_Camera,
	Obj_Player,
	Obj_Bullet,
	Obj_Enemy,
	Obj_Test,
	ObjTypeMax
};

typedef struct _ObjectFunc
{
	void(*init)(Object* thiz);
	void(*update)(Object* thiz);
	void(*uninit)(Object* thiz);
	void(*onCollision)(Object* thiz, Object* other);
}ObjectFunc;


void InitObjectType(void);
ObjectFunc* GetObjectFunc(ObjectType type);