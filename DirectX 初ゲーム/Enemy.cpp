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
		// �^�[�Q�b�g�Ƃ̃x�N�g�����v�Z����
		Vector3 dis = this->target->position - this->transform->position;
		// �O�ς��v�Z����
		float cross = asinf(Vector2::Cross(this->transform->getUp().toVector2(), dis.toVector2()) / dis.length() );
		// �O�ς̌��ʂɂ���]������������߂�
		this->transform->rotate(0, 0, 0.02f * ((cross > 0) ? 1 : -1));

		// �����̑O���֐i��
		this->transform->position += this->transform->getUp() * ENEMY_SPEED * Time::DeltaTime();

		// ���̎��Ԃɒe����
		if (this->timer > 0.5f)
		{
			new Bullet(this, 300 * this->transform->getUp());
			this->timer = 0; // �^�C�}�[�̃��Z�b�g
		}

		// �^�C�}�[�̍X�V
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
