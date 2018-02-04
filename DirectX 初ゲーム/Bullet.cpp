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

	this->timer = 0;

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
	this->timer += Time::DeltaTime();

	if (this->timer > 3.0f)
		this->Destroy();
}

void Bullet::OnCollision(Object * other)
{

	if (this->type == ObjectType::Bullet && (other->type == ObjectType::Enemy /*|| other->type == ObjectType::Bullet_E*/))
	{
		this->Destroy();
	}
	else if (this->type == ObjectType::Bullet_E && (other->type == ObjectType::Player /*|| other->type == ObjectType::Bullet*/))
	{
		this->Destroy();
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
