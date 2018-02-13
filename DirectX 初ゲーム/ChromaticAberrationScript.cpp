#include "ChromaticAberrationScript.h"

ChromaticAberrationScript::ChromaticAberrationScript(void)
{
	this->active_on_load = false;
	this->interval = 0.0f;
	this->pause = false;
}

void ChromaticAberrationScript::Init(void)
{
	this->ca = this->object->GetComponent<ChromaticAberration>();
	if (this->ca == nullptr)
	{
		SetActive(false);
		return;
	}

	this->ca->strength = 0.0f;
	this->timer.Reset(this->interval);
}

void ChromaticAberrationScript::Update(void)
{

	if (this->timer.TimeUp())
	{
		this->ca->strength = 0.0f;
		SetActive(false);
		return;
	}

	if (this->timer.Progress() < 0.8f)
		this->ca->strength = Lerpf(this->ca->strength, 1.0f, 0.1f);
	else
		this->ca->strength = Lerpf(this->ca->strength, 0.0f, 0.2f);

	this->ca->time = this->timer.Elapsed();

	if(!this->pause)
		this->timer++;
}

void ChromaticAberrationScript::Uninit(void)
{
	this->interval = 0.0f;
}
