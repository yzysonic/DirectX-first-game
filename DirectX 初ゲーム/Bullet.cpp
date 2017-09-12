#include "Bullet.h"

static Bullet *list[BULLET_MAX];
static int listTop = -1;

Bullet * newBullet(const Transform * transform, Vector3 velocity)
{
	if (listTop == BULLET_MAX - 1)
		return NULL;

	Bullet* thiz = list[++listTop] = NewSubObj(Bullet);

	thiz->base->rigidbody->position = transform->position;
	thiz->base->rigidbody->rotation = transform->rotation;
	thiz->base->rigidbody->velocity = velocity;

	if (transform->object->type == Obj_Player)
	{
		thiz->base->polygon = newPolygon(thiz->base, LAYER_DEFAULT, TEX_BULLET);
		SetVolume(SE_BULLET, -1000);
		PlaySE(SE_BULLET);
	}
	else
	{
		thiz->base->type = Obj_Bullet_E;
		thiz->base->polygon = newPolygon(thiz->base, LAYER_DEFAULT, TEX_BULLET_E);
	}

	thiz->index = listTop;
	thiz->timer = 0;


	return thiz;
}

void initBullet(Object * thiz)
{
	thiz->rigidbody = newRigidbody(thiz);
	thiz->rigidbody->useGravity = false;
	thiz->collider = newCollider(thiz);
}

void updateBullet(Object * thiz)
{
	SetThis(Bullet);
	thizz->timer += GetDeltaTime();

	if (thizz->timer > 1.5f)
		DeleteSubObj(thizz);
}

void uninitBullet(Object * thiz)
{
	SetThis(Bullet);

	int index = thizz->index;

	if (index < listTop)
	{
		list[index] = list[listTop];
		list[index]->index = index;
	}

	list[listTop] = NULL;
	listTop--;
}

void onCollisionBullet(Object * thiz, Object * other)
{
	SetThis(Bullet);

	if (thiz->type == Obj_Bullet && other->type == Obj_Enemy)
	{
		Polygon_SetOpacity(thiz->polygon, 0.0f);
		DeleteSubObj(thizz);
	}
	else if (thiz->type == Obj_Bullet_E && other->type == Obj_Player)
	{
		Polygon_SetOpacity(thiz->polygon, 0.0f);
		DeleteSubObj(thizz);
	}

}

void CleanBullets()
{
	for (int i = listTop; i >= 0; i--)
		DeleteSubObj(list[i]);
}
