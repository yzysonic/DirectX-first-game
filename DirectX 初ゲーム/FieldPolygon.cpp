#include "FieldPolygon.h"

FieldPolygon::FieldPolygon(int max):FieldPolygon(max, (float)SystemParameters::ResolutionX, (float)SystemParameters::ResolutionY, 15.0f, 1000.0f, 0.7f){}

FieldPolygon::FieldPolygon(int max, float size_x, float size_y, float scale, float emission_rate, float opacity)
{
	this->field_size.x = size_x;
	this->field_size.y = size_y;
	this->scale = scale;
	this->opacity = opacity;

	auto particle =
		this->AddComponent<ParticleSystem>(max, this);

	particle->SetDuration(-1.0f);
	particle->emission_rate = emission_rate;
}

bool FieldPolygon::IsCompelete(void)
{
	return GetComponent<ParticleSystem>()->GetParticleNum() == GetComponent<ParticleSystem>()->GetMaxNum();
}


void FieldPolygon::Init(ParticleElement & element)
{
	element.transform.scale = Vector3::one*this->scale;

	element.transform.position.x = (float)Random(-(int)this->field_size.x / 2, (int)this->field_size.x / 2);
	element.transform.position.y = (float)Random(-(int)this->field_size.y / 2, (int)this->field_size.y / 2);
	element.transform.position.z = Randomf(-150.0f, 1000.0f);

	element.color = Color(Random(0, 255), Random(0, 255), Random(0, 255), (UCHAR)(this->opacity*255));
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
