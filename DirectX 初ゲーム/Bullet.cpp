#include "Bullet.h"

Bullet::Bullet(Object * owner, Vector3 velocity)
{
	// 座標、回転初期化
	this->transform.position = owner->transform.position;
	this->transform.setRotation(0.0f, 0.0f, atan2f(velocity.y, velocity.x)-0.5f*PI);

	// 描画コンポネント初期化
	this->AddComponent<Rigidbody>();
	this->GetComponent<Rigidbody>()->useGravity = false;
	this->GetComponent<Rigidbody>()->velocity = velocity;

	// 発射元で処理を分ける
	if (owner->type == ObjectType::Player)
	{
		this->type = ObjectType::Bullet;
		this->AddComponent<RectPolygon>("bullet", Layer::BULLET);
		this->AddComponent<SphereCollider>();
		this->GetComponent<SphereCollider>()->radius = 10.0f;
		SetVolume(SE_BULLET, -1000);
		PlaySE(SE_BULLET);
	}
	else
	{
		this->type = ObjectType::Bullet_E;
		this->AddComponent<RectPolygon>("bullet_e", Layer::BULLET);
		this->AddComponent<SphereCollider>();
		this->GetComponent<SphereCollider>()->radius = 15.0f;
	}


	this->timer.Reset(LifeTime-WeakTime);
	this->state = Normal;

}


void Bullet::Update()
{
	switch (this->state)
	{
	case Normal:
		if (this->timer.TimeUp())
		{
			this->timer.Reset(WeakTime);
			this->state = Weak;
		}
		break;

	case Collision:
		if (this->timer.TimeUp())
			this->Destroy();
		break;

	case Weak:
		GetComponent<RectPolygon>()->SetOpacity(Lerpf(1.0f, 0.0f, this->timer.Progress()));
		if (this->timer.Progress() >= 0.5f)
			this->GetComponent<SphereCollider>()->SetActive(false);
		if (this->timer.TimeUp())
			this->Destroy();
		break;
	}
	

	this->timer++;
}

void Bullet::OnDraw(void)
{
	//auto pDevice = Direct3D::GetDevice();

	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

}

void Bullet::AfterDraw(void)
{
	//auto pDevice = Direct3D::GetDevice();

	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
}

void Bullet::OnCollision(Object * other)
{

	if (this->type == ObjectType::Bullet && (other->type == ObjectType::Enemy /*|| other->type == ObjectType::Bullet_E*/))
	{
		SetStateToCollision();
	}
	else if (this->type == ObjectType::Bullet_E && (other->type == ObjectType::Player /*|| other->type == ObjectType::Bullet*/))
	{
		SetStateToCollision();
	}

}

void Bullet::SetStateToCollision(void)
{
	this->GetComponent<RectPolygon>()->SetActive(false);
	this->GetComponent<Rigidbody>()->SetActive(false);
	this->GetComponent<SphereCollider>()->SetActive(false);

	auto particle =
	this->AddComponent<ParticleSystem>(20);
	particle->SetDuration(1.3f);
	particle->emission_rate = 10000.0f;

	auto behavior = particle->GetBehavior<ParticleDefaultBehavior>();
	if(this->type == ObjectType::Bullet)
		behavior->start_color = Color(255, 254, 191, 255);
	else
		behavior->start_color = Color(253, 226, 255, 255);
	behavior->start_speed = 5.0f;
	behavior->start_size = 5.0f;
	behavior->end_size = 1.0f;

	this->state = Collision;
	this->timer.Reset(1.0f);
}