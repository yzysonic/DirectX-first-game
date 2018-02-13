#include "Bullet.h"


std::list<Bullet*> Bullet::list;

Bullet::Bullet(Object * owner, Vector3 velocity)
{
	this->transform.position = owner->transform.position;
	this->transform.setRotation(0.0f, 0.0f, atan2f(velocity.y, velocity.x)-0.5f*PI);

	this->AddComponent<Rigidbody>();
	this->GetComponent<Rigidbody>()->useGravity = false;
	this->GetComponent<Rigidbody>()->velocity = velocity;

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

	this->timer.Reset(3.0f);
	this->state = 0;
	this->index = Bullet::list.size();
	Bullet::list.push_back(this);

}

Bullet::~Bullet(void)
{
	Bullet::list.back()->index = this->index;
	*std::next(Bullet::list.begin(), this->index) = Bullet::list.back();
	Bullet::list.pop_back();
}


void Bullet::Update()
{

	if (this->timer.TimeUp())
		this->Destroy();

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
		ToState1();
	}
	else if (this->type == ObjectType::Bullet_E && (other->type == ObjectType::Player /*|| other->type == ObjectType::Bullet*/))
	{
		ToState1();
	}

}

void Bullet::Clear()
{
	//std::list<Bullet*> tList;
	//for (auto bullet : Bullet::list)
	//	tList.push_back(bullet);

	//for (auto bullet : tList)
	//	delete bullet;
}

void Bullet::ToState1(void)
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

	this->state = 1;
	this->timer.Reset(1.0f);
}
