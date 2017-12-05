#include "Bullet.h"


std::list<Bullet*> Bullet::list;

Bullet::Bullet(Object * owner, Vector3 velocity) : Object2D(owner->getTransform()->position, owner->getTransform()->getRotation())
{
	this->setCollider();

	this->setRigidbody();
	this->rigidbody->useGravity = false;
	this->rigidbody->velocity = velocity;

	if (owner->type == ObjectType::Player)
	{
		this->type = ObjectType::Bullet;
		this->setPolygon(Layer::DEFAULT, TEX_BULLET);
		SetVolume(SE_BULLET, -1000);
		PlaySE(SE_BULLET);
	}
	else
	{
		this->type = ObjectType::Bullet_E;
		this->setPolygon(Layer::DEFAULT, TEX_BULLET_E);
		this->collider->size *= 0.5f;
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


void Bullet::update()
{
	this->timer += Time::DeltaTime();

	if (this->timer > 3.0f)
		delete this;
}

void Bullet::onCollision(Object * other)
{

	if (this->type == ObjectType::Bullet && (other->type == ObjectType::Enemy /*|| other->type == ObjectType::Bullet_E*/))
	{
		delete this;
	}
	else if (this->type == ObjectType::Bullet_E && (other->type == ObjectType::Player /*|| other->type == ObjectType::Bullet*/))
	{
		delete this;
	}

}

void Bullet::Clear()
{
	std::list<Bullet*> tList;
	for (auto bullet : Bullet::list)
		tList.push_back(bullet);

	for (auto bullet : tList)
		delete bullet;
}
