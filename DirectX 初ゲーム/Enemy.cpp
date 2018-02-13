#include "Enemy.h"
#include "SceneGame.h"


Enemy::Enemy()
{
	// �|���S���̏�����
	AddComponent<RectPolygon>("enemy", Layer::DEFAULT);

	// �R���C�_�[�̏�����
	AddComponent<SphereCollider>();
	GetComponent<SphereCollider>()->radius = 35.0f;
	GetComponent<SphereCollider>()->SetActive(false);


	this->type = ObjectType::Enemy;
	this->hp = MaxHP;
	this->timer = 0;
	this->effect_timer = EffectTime+1.0f;

	Enemy::pUpdate = &Enemy::update_init;
}

void Enemy::Update()
{
	(this->*pUpdate)();

	// �^�C�}�[�̍X�V
	this->timer += Time::DeltaTime();

}

void Enemy::OnCollision(Object * other)
{
	if (other->type == ObjectType::Bullet)
	{
		this->hp--;
		float x = 1.0f - 0.5f*((float)(hp) / MaxHP);
		GetComponent<RectPolygon>()->SetColor(Color(255, (UCHAR)(255 * x), (UCHAR)(255 * x), 255));
		this->effect_timer = 0.0f;
		this->injury();
	}
	//if (other->type == ObjectType::Enemy)
	//{
	//	this->transform.position += (this->transform.position - other->transform.position).normalized()*2.0f;
	//}
}

void Enemy::SetDeath(void)
{
	this->GetComponent<RectPolygon>()->SetActive(false);
	this->GetComponent<SphereCollider>()->SetActive(false);

	auto particle =
	this->AddComponent<ParticleSystem>(200);
	particle->SetDuration(0.3f);
	particle->emission_rate = 1000.0f;

	auto behavior = particle->GetBehavior<ParticleDefaultBehavior>();
	behavior->start_color = Color(243, 124, 125, 255);
	behavior->start_size = 6.0f;
	behavior->end_size = 1.0f;
	behavior->start_speed = 10.0f;

	this->timer = 0.0f;
	
	Enemy::pUpdate = &Enemy::update_death;
}

void Enemy::update_init(void)
{
	float progress = (this->timer / InitTime);

	if (this->timer > InitTime)
	{
		this->timer = 0.0f;
		progress = 1.0f;
		this->transform.scale = Vector3::one;
		GetComponent<SphereCollider>()->SetActive(true);
		Enemy::pUpdate = &Enemy::update_main;
	}


	this->transform.scale = Ease::QuadIn(0.2f, 1.0f, progress)*Vector3::one;
	this->transform.rotate(0.0f, 0.0f, Ease::QuadIn(Time::DeltaTime()*10.0f*PI, 0.0f, progress));
	GetComponent<RectPolygon>()->SetOpacity(Lerpf(0.3f, 1.0f, progress));

	if (progress > 0.8f)
	{
		this->transform.scale += 0.6f*sinf((progress - 0.8f)/0.2f*PI)*Vector3::one;

	}
	
}

void Enemy::update_main(void)
{
	if (this->target)
	{
		// �^�[�Q�b�g�Ƃ̃x�N�g�����v�Z����
		Vector3 dis = this->target->position - this->transform.position;
		// �O�ς��v�Z����
		float cross = asinf(Vector2::Cross(this->transform.getUp().toVector2(), dis.toVector2()) / dis.length());
		// �O�ς̌��ʂɂ���]������������߂�
		this->transform.rotate(0, 0, 0.02f * ((cross > 0) ? 1 : -1));

		// �����̑O���֐i��
		this->transform.position += this->transform.getUp() * Speed * Time::DeltaTime();

		// ���̎��Ԃɒe����
		if (this->timer > 0.5f)
		{
			/*this->scene->AddObject*/(new Bullet(this, 300 * this->transform.getUp()));
			this->timer = 0; // �^�C�}�[�̃��Z�b�g
		}
	}

	if (this->effect_timer <= EffectTime+0.3f)
	{
		float p = (1.0f* (float)(this->hp) / MaxHP - 0.3f*sinf(this->effect_timer / EffectTime*PI));
		
		if (this->effect_timer < EffectTime)
		{
			GetComponent<RectPolygon>()->SetColor(Color(255, (UCHAR)(255 * p), (UCHAR)(255 * p), 255));
		}
		else
		{
			float x = 1.0f*((float)(hp) / MaxHP);
			GetComponent<RectPolygon>()->SetColor(Color(255, (UCHAR)(255 * x), (UCHAR)(255 * x), 255));
		}
		this->effect_timer += Time::DeltaTime();
	}
}

void Enemy::update_death(void)
{
	if (this->timer >= 1.0f)
		this->Destroy();
}
