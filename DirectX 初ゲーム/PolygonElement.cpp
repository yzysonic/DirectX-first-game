#include "PolygonElement.h"

PolygonElement::PolygonElement()
{
	this->type = ObjectType::PolygonElement;
	this->setPolygon();
	this->transform->scale = Vector3(0.1f, 0.1f, 1.0f);

	this->transform->position.x = Randomf(-SCREEN_CENTER_X - 100, SCREEN_CENTER_X + 100);
	this->transform->position.y = Randomf(-SCREEN_CENTER_Y - 100, SCREEN_CENTER_Y + 100);
	this->transform->position.z = Randomf(0.01f, 5);

	this->timer = 0;
	this->timerInterval = Randomf(0.3f, 1.0f);
	this->speed = Vector3(Randomf(-1, 1), Randomf(-1, 1), 0.0f)*30.0f;
	this->targetOpacity = 1.0f;

	this->polygon->setColor(ColorRGBA(Random(0, 255), Random(0, 255), Random(0, 255), 0));

	PolygonElement::pUpdate = &PolygonElement::update_state0;
}

void PolygonElement::update()
{
	(this->*pUpdate)();
}


void PolygonElement::update_state0()
{
	float interval = 0.2f;
	if (this->timer < interval)
	{
		this->polygon->setOpacity(this->targetOpacity*(this->timer / interval));
		this->transform->scale.x = Lerpf(0.1f, 0.4f, (this->timer / interval));
		this->transform->scale.y = Lerpf(0.1f, 0.4f, (this->timer / interval));
		this->timer += Time::DeltaTime();
	}
	else
	{
		PolygonElement::pUpdate = &PolygonElement::update_state1;
	}
}

void PolygonElement::update_state1()
{
	if (this->timer >= this->timerInterval)
	{
		this->speed = Vector3(Randomf(-1, 1), Randomf(-1, 1), 0.0f)*30.0f;
		this->timerInterval = Randomf(0.2f, 0.7f);
		this->timer = 0;
	}

	this->transform->position += this->speed*Time::DeltaTime();

	this->timer += Time::DeltaTime();
}
