#include "Enemy.h"
#include "SceneGame.h"

Enemy::Enemy()
{
	this->type = ObjectType::Enemy;
	this->setPolygon(Layer::DEFAULT, TEX_ENEMY);
	this->setCollider();
	this->collider->size.x -= 50;
	this->collider->size.y -= 50;

	this->hp = 3;
	this->timer = 0;
}

void Enemy::update()
{

	if (this->target)
	{
		// ターゲットとのベクトルを計算する
		Vector3 dis = this->target->position - this->transform->position;
		// 外積を計算する
		float cross = asinf(Vector2::Cross(this->transform->getUp().toVector2(), dis.toVector2()) / dis.length() );
		// 外積の結果により回転する方向を決める
		this->transform->rotate(0, 0, 0.02f * ((cross > 0) ? 1 : -1));

		// 自分の前方へ進む
		this->transform->position += this->transform->getUp() * ENEMY_SPEED * Time::DeltaTime();

		// 一定の時間に弾発射
		if (this->timer > 0.5f)
		{
			new Bullet(this, 300 * this->transform->getUp());
			this->timer = 0; // タイマーのリセット
		}

		// タイマーの更新
		this->timer += Time::DeltaTime();
	}

}

void Enemy::onCollision(Object2D * other)
{
	if (other->type == ObjectType::Bullet)
	{
		this->hp--;
	}
	//if (other->type == ObjectType::Enemy)
	//{
	//	this->transform->position += (this->transform->position - other->getTransform()->position).normalized()*2.0f;
	//}
}
