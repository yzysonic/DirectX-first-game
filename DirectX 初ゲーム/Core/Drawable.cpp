#include "Drawable.h"
#include "Object.h"
#include "Polygon.h"
#include "Renderer.h"
#include "RenderSpace.h"


Drawable::Drawable(Layer layer, std::string render_space)
{
	this->layer = layer;
	this->render_space = render_space;
	this->render_space_index = RenderSpace::Get(render_space)->GetIndex();
	this->visible = true;
	RenderSpace::Get(render_space)->AddDraw(this);
}

Drawable::~Drawable(void)
{
	RenderSpace::Get(render_space)->RemoveDraw(this);
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

void Drawable::setVisibility(bool visible)
{
	if (visible == this->visible)
		return;

	if(visible)
		RenderSpace::Get(this->render_space)->AddDraw(this);
	else
		RenderSpace::Get(render_space)->RemoveDraw(this);

	this->visible = visible;
}
