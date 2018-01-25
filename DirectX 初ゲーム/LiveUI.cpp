#include "LiveUI.h"

LiveUI::LiveUI(void)
{
	AddComponent<RectPolygon2D>("lives", Layer::UI_00);
	GetComponent<RectPolygon2D>()->setPattern(2);
	this->low_live = false;
	this->timer = 0.0f;
}

LiveUI::~LiveUI(void)
{
}

void LiveUI::Update(void)
{
	if (low_live)
	{
		UCHAR c = (UCHAR)(255*fabsf(sinf(this->timer*PI / AnimeTime)));
		GetComponent<RectPolygon2D>()->SetColor(Color(255, c, c, 255));

		this->timer += Time::DeltaTime();
	}
}

void LiveUI::SetLowLive(bool value)
{
	if (value == this->low_live)
		return;

	if (value)
		this->timer = 0.0f;
	else
		GetComponent<RectPolygon2D>()->SetColor(Color::white);

	this->low_live = value;
}
