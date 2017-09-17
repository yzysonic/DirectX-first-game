#include "Enemy.h"
#include "SceneGame.h"

Enemy::Enemy()
{
	this->type = ObjectType::Enemy;
	this->setPolygon(Layer::DEFAULT, TEX_ENEMY);
	this->setCollider();
	this->collider->size.x -= 100;
	this->collider->size.y -= 100;

	this->hp = 10;
}

void Enemy::update()
{

	if (this->target)
	{
		Vector3 dir;
		D3DXVec3Normalize(&dir, &(this->target->position - this->transform->position));
		this->transform->position += dir * ENEMY_SPEED * Time::DeltaTime();
	}

}

void Enemy::onCollision(Object * other)
{
	if (other->type == ObjectType::Bullet)
	{
		this->hp--;
	}
}
