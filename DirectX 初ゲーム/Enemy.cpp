#include "Enemy.h"
#include "SceneGame.h"

Enemy::Enemy()
{
	this->type = ObjectType::Enemy;
	this->setPolygon(Layer::DEFAULT, TEX_ENEMY);
	this->setCollider();
	this->collider->size.x -= 100;
	this->collider->size.y -= 100;

	this->hp = 3;
	this->timer = 0;
}

void Enemy::update()
{

	if (this->target)
	{
		Vector3 dir = (this->target->position - this->transform->position).normalized();
		this->transform->position += dir * ENEMY_SPEED * Time::DeltaTime();
		this->transform->lookAt(this->target);

		if (this->timer > 0.5f)
		{
			new Bullet(this, 300 * Vector3(cosf(this->transform->getRotation().z - PI/2), sinf(this->transform->getRotation().z - PI / 2), 0));
			this->timer = 0;
		}

		this->timer += Time::DeltaTime();
	}

}

void Enemy::onCollision(Object * other)
{
	if (other->type == ObjectType::Bullet)
	{
		this->hp--;
	}
}
