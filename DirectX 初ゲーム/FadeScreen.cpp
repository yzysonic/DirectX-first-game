#include "FadeScreen.h"

FadeScreen::FadeScreen(void)
{
	this->setPolygon(Layer::TOP, Texture::none, RendererType::UI);

	this->polygon->setSize((float)SystemParameters::ResolutionX, (float)SystemParameters::ResolutionY);
	this->polygon->setOpacity(0.0f);

	state = Stop;

	this->setActive(false);
}

void FadeScreen::update()
{

	switch (state)
	{
	case Set:
		this->polygon->setOpacity(targetOpacity);
		state = Stop;
		break;
	case Run:
		if (timer < fadeTime + 0.1f)
		{
			float t = this->polygon->getOpacity();
			this->polygon->setOpacity(Lerpf(oldOpacity, targetOpacity, timer / fadeTime));
		}
		else
		{
			state = Stop;
		}
		timer += Time::DeltaTime();
		break;
	case Stop:
		this->setActive(false);
		break;
	}

}

void FadeScreen::Fade(FadeType type, Color color, float interval)
{
	switch (type)
	{
	case FADE_IN:
		m_pInstance->targetOpacity = 0.0f;
		m_pInstance->polygon->setColor(color);
		m_pInstance->polygon->setOpacity(1.0f);
		break;
	case FADE_OUT:
		m_pInstance->targetOpacity = 1.0f;
		m_pInstance->polygon->setColor(color);
		m_pInstance->polygon->setOpacity(0.0f);
		break;
	}

	m_pInstance->fadeTime = interval;

	if (m_pInstance->fadeTime > 0)
		m_pInstance->state = Run;
	else
		m_pInstance->state = Set;

	m_pInstance->timer = 0;
	m_pInstance->oldOpacity = m_pInstance->polygon->getOpacity();
	m_pInstance->setActive(true);
}

void FadeScreen::FadeIn(Color color, float interval)
{
	Fade(FADE_IN, color, interval);
}

void FadeScreen::FadeOut(Color color, float interval)
{
	Fade(FADE_OUT, color, interval);
}

bool FadeScreen::Finished(void)
{
	if (m_pInstance->state == Stop)
		return true;
	else
		return false;
}
