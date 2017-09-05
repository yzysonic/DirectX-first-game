#include "ObjectType.h"
#include "Object.h"

#include "PolygonElement.h"
#include "Camera.h"
#include "Player.h"
#include "Bullet.h"
#include "test.h"

ObjectTypeFunc g_ObjectTypeFuncList[ObjTypeMax];

void EmptyFunc(Object *) {}
void EmptyFunc2(Object*, Object*) {};

void InitObjectType(void)
{
	for (int i = 0; i < ObjTypeMax; i++)
	{
		g_ObjectTypeFuncList[i].init = &EmptyFunc;
		g_ObjectTypeFuncList[i].update = &EmptyFunc;
		g_ObjectTypeFuncList[i].uninit = &EmptyFunc;
		g_ObjectTypeFuncList[i].onCollision = &EmptyFunc2;
	}

	g_ObjectTypeFuncList[Obj_PolygonElement].init = &initPolygonElement;
	g_ObjectTypeFuncList[Obj_PolygonElement].update = &updatePolygonElement;
	g_ObjectTypeFuncList[Obj_PolygonElement].uninit = &uninitPolygonElement;

	g_ObjectTypeFuncList[Obj_Camera].init = &initCamera;
	g_ObjectTypeFuncList[Obj_Camera].update = &updateCamera;
	g_ObjectTypeFuncList[Obj_Camera].uninit = &uninitCamera;

	g_ObjectTypeFuncList[Obj_Player].init = &initPlayer;
	g_ObjectTypeFuncList[Obj_Player].update = &updatePlayer;
	g_ObjectTypeFuncList[Obj_Player].uninit = &uninitPlayer;
	g_ObjectTypeFuncList[Obj_Player].onCollision = &onCollisionPlayer;

	g_ObjectTypeFuncList[Obj_Bullet].init = &initBullet;
	g_ObjectTypeFuncList[Obj_Bullet].update = &updateBullet;
	g_ObjectTypeFuncList[Obj_Bullet].uninit = &uninitBullet;

	g_ObjectTypeFuncList[Obj_Test].init = &initTest;
	g_ObjectTypeFuncList[Obj_Test].update = &updateTest;
	g_ObjectTypeFuncList[Obj_Test].uninit = &uninitTest;

}

ObjectTypeFunc * GetObjectTypeFunc(ObjectType type)
{
	return &g_ObjectTypeFuncList[type];
}
