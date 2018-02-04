#include "Drawable.h"
#include "Object.h"
#include "Polygon.h"
#include "Renderer.h"
#include "RenderSpace.h"


Drawable::Drawable(Layer layer = Layer::DEFAULT, std::string render_space_name = "default")
{
	this->layer = layer;
	this->render_space = render_space_name;
	this->render_space_index = -1;
	this->vshader = &VertexShader::default_shader;
	this->pshader = &PixelShader::default_shader;
}

Drawable::~Drawable(void)
{
	auto rs = RenderSpace::Get(render_space);
	if (rs) rs->RemoveDraw(this);
}

bool Drawable::SetActive(bool value)
{
	if (Component::SetActive(value) == false)
		return false;

	auto rs = RenderSpace::Get(this->render_space);

	if (rs)
	{
		if (value)
			rs->AddDraw(this);
		else
			rs->RemoveDraw(this);
	}

	return true;
}

void Drawable::SetRenderSpace(std::string render_space)
{
	SetActive(false);
	this->render_space = render_space;
	SetActive(true);
}

Layer Drawable::GetLayer(void)
{
	return this->layer;
}

void Drawable::SetLayer(Layer layer)
{
	SetActive(false);
	this->layer = layer;
	SetActive(true);
}

void Drawable::SetColor(Color color)
{
	this->color = color;
}

Color Drawable::GetColor(void)
{
	return this->color;
}

float Drawable::GetOpacity(void)
{
	return (float)(this->color.a) / 0xff;
}

void Drawable::SetOpacity(float opacity)
{
	this->color.a = (unsigned char)(opacity * 0xff);
	this->SetColor(this->color);
}