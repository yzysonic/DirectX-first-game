#include "Bullet.h"

std::list<Bullet*> Bullet::list;

Bullet::Bullet(Object * owner, Vector3 velocity)
{
	this->setCollider();

	this->setRigidbody();
	this->rigidbody->useGravity = false;
	this->rigidbody->position = owner->getTransform()->position;
	this->rigidbody->rotation = owner->getTransform()->rotation;
	this->rigidbody->velocity = velocity;

	if (owner->getTransform()->object->type == ObjectType::Player)
	{
		this->setPolygon(Layer::DEFAULT, TEX_BULLET);
		SetVolume(SE_BULLET, -1000);
		PlaySE(SE_BULLET);
	}
	else
	{
		this->type = ObjectType::Bullet_E;
		this->setPolygon(Layer::DEFAULT, TEX_BULLET_E);
	}

	this->timer = 0;

	this->index = Bullet::list.size();
	Bullet::list.push_back(this);

}

Bullet::~Bullet(void)
{
	Bullet::list.erase(std::next(Bullet::list.begin(), this->index));
}


void Bullet::update()
{
	this->timer += Time::DeltaTime();

	if (this->timer > 1.5f)
		delete this;
}

void Bullet::onCollision(Object * other)
{

	if (this->type == ObjectType::Bullet && other->type == ObjectType::Enemy)
	{
		this->polygon->setOpacity(0.0f);
		delete this;
	}
	else if (this->type == ObjectType::Bullet_E && other->type == ObjectType::Player)
	{
		this->polygon->setOpacity(0.0f);
		delete this;
	}

}

void Bullet::Clear()
{
	for (auto bullet : Bullet::list)
	{
		delete bullet;
	}
	Bullet::list.clear();
}
