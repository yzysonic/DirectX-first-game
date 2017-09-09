#include "Enemy.h"
#include "SceneGame.h"

void initEnemy(Object * thiz)
{
	SetThis(Enemy);

	thiz->polygon = newPolygon(thiz, LAYER_DEFAULT, TEX_ENEMY);
	thiz->collider = newCollider(thiz);

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

	if (thizz->hp == 0)
	{
		AddGameScore(300);
		DeleteSubObj(thizz);
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
