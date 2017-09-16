#include "FadeScreen.h"

FadeScreen::FadeScreen(void)
{
	this->setPolygon(Layer::TOP, TEX_NONE, RendererType::UI);

	this->polygon->setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
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
		break;
	case FADE_OUT:
		m_pInstance->targetOpacity = 1.0f;
		m_pInstance->polygon->setColor(color);
		break;
	case FADE_IN_BK:
		m_pInstance->targetOpacity = 0.0f;
		m_pInstance->polygon->setColor(ColorRGBA(0, 0, 0, 255));
		break;
	case FADE_OUT_BK:
		m_pInstance->targetOpacity = 1.0f;
		m_pInstance->polygon->setColor(ColorRGBA(0, 0, 0, 0));
		break;
	case FADE_IN_WH:
		m_pInstance->targetOpacity = 0.0f;
		m_pInstance->polygon->setColor(ColorRGBA(255, 255, 255, 255));
		break;
	case FADE_OUT_WH:
		m_pInstance->targetOpacity = 1.0f;
		m_pInstance->polygon->setColor(ColorRGBA(255, 255, 255, 0));
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

bool FadeScreen::Finished(void)
{
	if (m_pInstance->state == Stop)
		return true;
	else
		return false;
}
