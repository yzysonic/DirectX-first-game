#include "ObjectType.h"
#include "Object.h"

#include "NumberUI.h"
#include "PolygonElement.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "test.h"

#define AssignObjFunc(type, func) ObjFuncList[Obj_##type].func = &##func##type;

static ObjectFunc ObjFuncList[ObjTypeMax];

void EmptyFunc(Object *) {}
void EmptyFunc2(Object*, Object*) {};

void InitObjectType(void)
{
	for (int i = 0; i < ObjTypeMax; i++)
	{
		ObjFuncList[i].init = &EmptyFunc;
		ObjFuncList[i].update = &EmptyFunc;
		ObjFuncList[i].uninit = &EmptyFunc;
		ObjFuncList[i].onCollision = &EmptyFunc2;
	}

	AssignObjFunc(NumberUI, update);
	AssignObjFunc(NumberUI, uninit);

	AssignObjFunc(PolygonElement, init);
	AssignObjFunc(PolygonElement, update);
	AssignObjFunc(PolygonElement, uninit);

	AssignObjFunc(Camera, init);
	AssignObjFunc(Camera, update);
	AssignObjFunc(Camera, uninit);

	AssignObjFunc(Player, init);
	AssignObjFunc(Player, update);
	AssignObjFunc(Player, uninit);
	AssignObjFunc(Player, onCollision);

	AssignObjFunc(Enemy, init);
	AssignObjFunc(Enemy, update);
	AssignObjFunc(Enemy, uninit);
	AssignObjFunc(Enemy, onCollision);

	AssignObjFunc(Bullet, init);
	AssignObjFunc(Bullet, update);
	AssignObjFunc(Bullet, uninit);
	AssignObjFunc(Bullet, onCollision);

	AssignObjFunc(Test, init);
	AssignObjFunc(Test, update);
	AssignObjFunc(Test, uninit);

}

ObjectFunc * GetObjectFunc(ObjectType type)
{
	return &ObjFuncList[type];
}
