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
	this->state = 0;
	this->timer.Reset(this->interval*0.3f);
}

void ChromaticAberrationScript::Update(void)
{
	switch (this->state)
	{
	case 0:
		if (this->timer.TimeUp())
		{
			this->timer.Reset(this->interval*0.7f);
			this->state = 1;
		}
		else
			this->ca->strength = Lerpf(this->ca->strength, 1.0f, 0.1f);
		break;
		
	case 1:
		if (this->timer.TimeUp())
		{
			this->ca->strength = 0.0f;
			SetActive(false);
		}
		else
			this->ca->strength = Lerpf(1.0f, 0.0f, this->timer.Progress());
		break;
	}

	if (!this->pause)
	{
		this->ca->time += Time::DeltaTime();
		this->timer++;
	}
}

void ChromaticAberrationScript::Uninit(void)
{
	this->interval = 0.0f;
}
