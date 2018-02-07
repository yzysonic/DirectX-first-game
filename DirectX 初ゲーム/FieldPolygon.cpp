#include "FieldPolygon.h"

FieldPolygon::FieldPolygon(int max)
{
	auto particle =
	this->AddComponent<ParticleSystem>(max, this);

	particle->SetDuration(-1.0f);
	particle->emission_rate = 1000.0f;
}


void FieldPolygon::Init(ParticleElement & element)
{
	element.transform.scale = Vector3(15.0f, 15.0f, 1.0f);

	element.transform.position.x = (float)Random(-SystemParameters::ResolutionX / 2 - 50, SystemParameters::ResolutionX / 2 + 50);
	element.transform.position.y = (float)Random(-SystemParameters::ResolutionY / 2 - 50, SystemParameters::ResolutionY / 2 + 50);
	element.transform.position.z = Randomf(-200.0f, 1000.0f);

	element.color = Color(Random(0, 255), Random(0, 255), Random(0, 255), 179);
}

void FieldPolygon::Update(ParticleElement & element)
{
	if (element.timer.TimeUp())
	{
		element.dir = Vector3(Randomf(-1, 1), Randomf(-1, 1), 0.0f)*30.0f;
		element.timer.Reset(Randomf(0.2f, 0.7f));
	}

	element.transform.position += element.dir*Time::DeltaTime();
	element.timer.Step();

}
