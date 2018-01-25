#include "FadeScreen.h"

FadeScreen::FadeScreen(void)
{
	this->AddComponent<RectPolygon2D>("none", Layer::TOP);

	this->GetComponent<RectPolygon2D>()->setSize((float)SystemParameters::ResolutionX, (float)SystemParameters::ResolutionY);
	this->GetComponent<RectPolygon2D>()->SetOpacity(0.0f);

	state = Stop;

	SetActive(false);
}

void FadeScreen::Update()
{

	switch (state)
	{
	case Set:
		this->GetComponent<RectPolygon2D>()->SetOpacity(targetOpacity);
		state = Stop;
		break;
	case Run:
		if (timer < fadeTime + 0.1f)
		{
			float t = this->GetComponent<RectPolygon2D>()->GetOpacity();
			this->GetComponent<RectPolygon2D>()->SetOpacity(Lerpf(oldOpacity, targetOpacity, timer / fadeTime));
		}
		else
		{
			state = Stop;
		}
		timer += Time::DeltaTime();
		break;
	case Stop:
		SetActive(false);
		break;
	}

}

void FadeScreen::SetActive(bool value)
{
	if (this->isActive == value)
		return;

	this->isActive = value;
}

void FadeScreen::Fade(FadeType type, Color color, float interval)
{
	switch (type)
	{
	case FADE_IN:
		m_pInstance->targetOpacity = 0.0f;
		m_pInstance->GetComponent<RectPolygon2D>()->SetColor(color);
		m_pInstance->GetComponent<RectPolygon2D>()->SetOpacity(1.0f);
		break;
	case FADE_OUT:
		m_pInstance->targetOpacity = 1.0f;
		m_pInstance->GetComponent<RectPolygon2D>()->SetColor(color);
		m_pInstance->GetComponent<RectPolygon2D>()->SetOpacity(0.0f);
		break;
	}

	m_pInstance->fadeTime = interval;

	if (m_pInstance->fadeTime > 0)
		m_pInstance->state = Run;
	else
		m_pInstance->state = Set;

	m_pInstance->timer = 0;
	m_pInstance->oldOpacity = m_pInstance->GetComponent<RectPolygon2D>()->GetOpacity();
	m_pInstance->SetActive(true);
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
