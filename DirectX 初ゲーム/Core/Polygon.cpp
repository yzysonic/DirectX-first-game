#include "Polygon.h"
#include "Object.h"
#include "Renderer.h"


RectPolygon::RectPolygon(Object* object, Layer layer, TextureName texName, RendererType rendType)
{

	this->object	= object;
	this->layer		= layer;
	this->rendType	= rendType;
	this->pTexture	= GetTexture(texName);
	if(this->pTexture->pDXTex)
		this->size	= this->pTexture->size;
	else
		this->size	= Vector2(100.f, 100.f);

	this->radius = this->size.length()/2;
	this->baseAngle = atan2f(this->size.y, this->size.x);
	this->color.setRGBA(255, 255, 255, 255);

	// rhwの設定
	this->vertex[0].rhw =
	this->vertex[1].rhw =
	this->vertex[2].rhw =
	this->vertex[3].rhw = 1.0f;

	// 反射光の設定
	this->vertex[0].diffuse =
	this->vertex[1].diffuse =
	this->vertex[2].diffuse =
	this->vertex[3].diffuse = this->color;

	// テクスチャ座標の設定
	this->vertex[0].uv = Vector2(0.0f,								0.0f);
	this->vertex[1].uv = Vector2(1.0f / this->pTexture->divideX,	0.0f);
	this->vertex[2].uv = Vector2(0.0f,								1.0f / this->pTexture->divideY);
	this->vertex[3].uv = Vector2(1.0f / this->pTexture->divideX,	1.0f / this->pTexture->divideY);

	this->pattern = 0;

	Renderer::GetInstance()->addList(this);
}

RectPolygon::~RectPolygon(void)
{
	Renderer::GetInstance()->removeList(this);
}

Layer RectPolygon::getLayer(void)
{
	return this->layer;
}

Vector2 RectPolygon::getSize(void)
{
	return this->size;
}

Color RectPolygon::getColor(void)
{
	return this->color;
}

void RectPolygon::setSize(float x, float y)
{
	this->size = Vector2(x, y);
	this->radius = this->size.length()/2;
	this->baseAngle = atan2f(this->size.y, this->size.x);
}


void RectPolygon::setColor(Color color)
{
	for (int i = 0; i < RECT_NUM_VERTEX; i++)
		this->vertex[i].diffuse = color;
	this->color = color;
}

void RectPolygon::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	for (int i = 0; i < RECT_NUM_VERTEX; i++)
		this->vertex[i].diffuse.setRGBA(r, g, b, a);
	this->color.setRGBA(r, g, b, a);
}

void RectPolygon::setOpacity(float opacity)
{
	this->color.a = (unsigned char)(opacity * 0xff);
	this->setColor(this->color);
}

float RectPolygon::getOpacity()
{
	return (float)(this->color.a)/0xff;
}

void RectPolygon::setPattern(unsigned int pattern)
{
	this->pattern = pattern;

	int x = pattern % this->pTexture->divideX;
	int y = pattern / this->pTexture->divideX;
	Vector2 size = Vector2(1.0f / this->pTexture->divideX, 1.0f / this->pTexture->divideY);

	this->vertex[0].uv = Vector2(x*size.x, y*size.y);
	this->vertex[1].uv = Vector2(x*size.x + size.x, y*size.y);
	this->vertex[2].uv = Vector2(x*size.x, y*size.y + size.y);
	this->vertex[3].uv = Vector2(x*size.x + size.x, y*size.y + size.y);
}
