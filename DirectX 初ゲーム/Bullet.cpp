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

	thiz->index = listTop;
	thiz->timer = 0;

	SetVolume(SE_BULLET, -1000);
	PlaySE(SE_BULLET);

	return thiz;
}

void initBullet(Object * thiz)
{
	thiz->polygon = newPolygon(thiz, LAYER_DEFAULT, TEX_BULLET);
	thiz->rigidbody = newRigidbody(thiz);
	thiz->rigidbody->useGravity = false;
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

void CleanBullets()
{
	for (int i = listTop; i >= 0; i--)
		DeleteSubObj(list[i]);
}
