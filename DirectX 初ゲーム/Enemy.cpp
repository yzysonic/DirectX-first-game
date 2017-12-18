#include "Enemy.h"
#include "SceneGame.h"


Enemy::Enemy()
{
	this->type = ObjectType::Enemy;
	this->setPolygon(Layer::DEFAULT, Texture::Get("enemy"));
	this->setCollider();
	this->collider->size.x -= 50;
	this->collider->size.y -= 50;

	this->hp = MaxHP;
	this->timer = 0;
	this->effect_timer = EffectTime+1.0f;

	Enemy::pUpdate = &Enemy::update_init;
}

void Enemy::update()
{
	(this->*pUpdate)();

	// タイマーの更新
	this->timer += Time::DeltaTime();

}

void Enemy::onCollision(Object2D * other)
{
	if (other->type == ObjectType::Bullet)
	{
		this->hp--;
		float x = 1.0f - 0.5f*((float)(hp) / MaxHP);
		this->polygon->setColor(255, (UCHAR)(255 * x), (UCHAR)(255 * x), 255);
		this->effect_timer = 0.0f;
	}
	//if (other->type == ObjectType::Enemy)
	//{
	//	this->transform->position += (this->transform->position - other->getTransform()->position).normalized()*2.0f;
	//}
}

void Enemy::update_init(void)
{
	float progress = (this->timer / InitTime);

	if (this->timer > InitTime)
	{
		Enemy::pUpdate = &Enemy::update_main;
		this->timer = 0.0f;
		progress = 1.0f;
		this->transform->scale = Vector3::one;
	}


	this->transform->scale = Ease::QuadIn(0.2f, 1.0f, progress)*Vector3::one;
	this->transform->rotate(0.0f, 0.0f, Ease::QuadIn(Time::DeltaTime()*10.0f*PI, 0.0f, progress));
	this->polygon->setOpacity(Lerpf(0.3f, 1.0f, progress));

	if (progress > 0.8f)
	{
		this->transform->scale += 0.6f*sinf((progress - 0.8f)/0.2f*PI)*Vector3::one;

	}
	
}

void Enemy::update_main(void)
{
	if (this->target)
	{
		// ターゲットとのベクトルを計算する
		Vector3 dis = this->target->position - this->transform->position;
		// 外積を計算する
		float cross = asinf(Vector2::Cross(this->transform->getUp().toVector2(), dis.toVector2()) / dis.length());
		// 外積の結果により回転する方向を決める
		this->transform->rotate(0, 0, 0.02f * ((cross > 0) ? 1 : -1));

		// 自分の前方へ進む
		this->transform->position += this->transform->getUp() * Speed * Time::DeltaTime();

		// 一定の時間に弾発射
		if (this->timer > 0.5f)
		{
			new Bullet(this, 300 * this->transform->getUp());
			this->timer = 0; // タイマーのリセット
		}
	}

	if (this->effect_timer <= EffectTime+0.3f)
	{
		float p = (1.0f* (float)(this->hp) / MaxHP - 0.3f*sinf(this->effect_timer / EffectTime*PI));
		
		if (this->effect_timer < EffectTime)
		{
			this->polygon->setColor(255, (UCHAR)(255 * p), (UCHAR)(255 * p), 255);
		}
		else
		{
			float x = 1.0f*((float)(hp) / MaxHP);
			this->polygon->setColor(255, (UCHAR)(255 * x), (UCHAR)(255 * x), 255);
		}
		this->effect_timer += Time::DeltaTime();
	}
}
