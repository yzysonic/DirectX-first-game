#include "PolygonElement.h"

PolygonElement::PolygonElement()
{
	this->type = ObjectType::PolygonElement;
	AddComponent<RectPolygon>("none", Layer::BG_02);
	this->transform.scale = Vector3(0.01f, 0.01f, 1.0f);

	this->transform.position.x = (float)Random(-SystemParameters::ResolutionX/2 - 100, SystemParameters::ResolutionX/2 + 100);
	this->transform.position.y = (float)Random(-SystemParameters::ResolutionY/2 - 100, SystemParameters::ResolutionY/2 + 100);
	this->transform.position.z = Randomf(-100.0f, 300.0f);

	this->timer = 0;
	this->timerInterval = Randomf(0.3f, 1.0f);
	this->speed = Vector3(Randomf(-1, 1), Randomf(-1, 1), 0.0f)*30.0f;
	this->targetOpacity = 1.0f;

	GetComponent<RectPolygon>()->SetColor(Color(Random(0, 255), Random(0, 255), Random(0, 255), 0));

	PolygonElement::pUpdate = &PolygonElement::update_state0;
}

void PolygonElement::Update()
{
	(this->*pUpdate)();
}


void PolygonElement::update_state0()
{
	float interval = 0.2f;
	if (this->timer < interval)
	{
		GetComponent<RectPolygon>()->SetOpacity(this->targetOpacity*(this->timer / interval));
		this->transform.scale.x = Lerpf(this->transform.scale.x, this->targetScale.x, (this->timer / interval));
		this->transform.scale.y = Lerpf(this->transform.scale.y, this->targetScale.y, (this->timer / interval));
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

	this->transform.position += this->speed*Time::DeltaTime();

	this->timer += Time::DeltaTime();
}
