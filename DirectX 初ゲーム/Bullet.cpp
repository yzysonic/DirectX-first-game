#include "Bullet.h"

void initBullet(Object * thiz)
{
	SetThis(Bullet);
	
	thiz->polygon = newPolygon(thiz, LAYER_DEFAULT, TEX_BULLET);
	thiz->rigidbody = newRigidbody(thiz);
	thiz->rigidbody->useGravity = false;
	thisBullet->timer = 0;

}

void updateBullet(Object * thiz)
{
	SetThis(Bullet);
	thisBullet->timer += GetDeltaTime();

	if (thisBullet->timer > 0.7f)
		DeleteSubObj(thisBullet);
}

void uninitBullet(Object * thiz)
{
}
