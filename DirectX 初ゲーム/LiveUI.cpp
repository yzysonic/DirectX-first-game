#include "LiveUI.h"

LiveUI::LiveUI(void)
{
	this->setPolygon(Layer::UI_00, Texture::Get("lives"), RendererType::UI);
	this->getPolygon()->setPattern(2);
	this->low_live = false;
	this->timer = 0.0f;
}

LiveUI::~LiveUI(void)
{
}

void LiveUI::update(void)
{
	if (low_live)
	{
		UCHAR c = (UCHAR)(255*fabsf(sinf(this->timer*PI / AnimeTime)));
		this->polygon->setColor(255, c, c, 255);

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
		this->polygon->setColor(255, 255, 255, 255);

	this->low_live = value;
}
