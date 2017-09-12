#include "Enemy.h"
#include "SceneGame.h"

void initEnemy(Object * thiz)
{
	SetThis(Enemy);

	thiz->polygon = newPolygon(thiz, LAYER_DEFAULT, TEX_ENEMY);
	thiz->collider = newCollider(thiz);
	thiz->collider->size.x -= 100;
	thiz->collider->size.y -= 100;

	thizz->hp = 10;
}

void updateEnemy(Object * thiz)
{
	SetThis(Enemy);
	
	if (thizz->target)
	{
		Vector3 dir;
		D3DXVec3Normalize(&dir, &(thizz->target->position - thiz->transform->position));
		thiz->transform->position += dir * ENEMY_SPEED * GetDeltaTime();
	}

}

void uninitEnemy(Object * thiz)
{
}

void onCollisionEnemy(Object * thiz, Object * other)
{
	SetThis(Enemy);

	if (other->type == Obj_Bullet)
	{
		thizz->hp--;
	}
}
