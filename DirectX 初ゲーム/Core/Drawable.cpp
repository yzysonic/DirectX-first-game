#include "Drawable.h"
#include "Object.h"
#include "Polygon.h"
#include "Renderer.h"

Drawable::~Drawable(void)
{
	Renderer::GetInstance()->removeList(this);
}

Layer Drawable::getLayer(void)
{
	return this->layer;
}

Color Drawable::getColor(void)
{
	return this->color;
}

void Drawable::setColor(Color color)
{
	this->color = color;
}

void Drawable::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->color.setRGBA(r, g, b, a);
}

float Drawable::getOpacity(void)
{
	return (float)(this->color.a) / 0xff;
}

void Drawable::setOpacity(float opacity)
{
	this->color.a = (unsigned char)(opacity * 0xff);
	this->setColor(this->color);
}
