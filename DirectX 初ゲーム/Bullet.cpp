#include "Bullet.h"

void initBullet(Object * thiz)
{
	SetThis(Bullet);
	
	thiz->polygon = newPolygon(thiz, LAYER_DEFAULT, TEX_BULLET);
	thiz->rigidbody = newRigidbody(thiz);
	thiz->rigidbody->useGravity = false;
	thizz->timer = 0;

}

void updateBullet(Object * thiz)
{
	SetThis(Bullet);
	thizz->timer += GetDeltaTime();

	if (thizz->timer > 0.7f)
		DeleteSubObj(thizz);
}

void uninitBullet(Object * thiz)
{
}
