#include "Polygon.h"
#include "Object.h"
#include "Renderer.h"


RectPolygon* newPolygon(Object* object, Layer layer, TextureName texName)
{
	RectPolygon *polygon = Renderer_GetPolygon(layer);

	polygon->object		= object;
	polygon->pTexture	= GetTexture(texName);
	if(polygon->pTexture->pDXTex)
		polygon->size	= polygon->pTexture->size;
	else
		polygon->size	= Vector2(100.f, 100.f);

	polygon->radius = D3DXVec2Length(&(polygon->size/2));
	polygon->baseAngle = atan2f(polygon->size.y, polygon->size.x);
	polygon->color = ColorRGBA(255, 255, 255, 255);

	// rhwの設定
	polygon->vertex[0].rhw =
	polygon->vertex[1].rhw =
	polygon->vertex[2].rhw =
	polygon->vertex[3].rhw = 1.0f;

	// 反射光の設定
	polygon->vertex[0].diffuse =
	polygon->vertex[1].diffuse =
	polygon->vertex[2].diffuse =
	polygon->vertex[3].diffuse = polygon->color;

	// テクスチャ座標の設定
	polygon->vertex[0].uv = Vector2(0.0f, 0.0f);
	polygon->vertex[1].uv = Vector2(1.0f, 0.0f);
	polygon->vertex[2].uv = Vector2(0.0f, 1.0f);
	polygon->vertex[3].uv = Vector2(1.0f, 1.0f);

	polygon->pattern = 0;

	return polygon;
}

void deletePolygon(RectPolygon * thiz)
{
	if (thiz == NULL) return;

	thiz->object = NULL;
	thiz->pTexture = NULL;
	Renderer_ReleasePolygon(thiz);
}


void Polygon_SetColor(RectPolygon * thiz, D3DCOLOR color)
{
	for (int i = 0; i < RECT_NUM_VERTEX; i++)
		thiz->vertex[i].diffuse = color;
	thiz->color = color;
}

void Polygon_SetOpacity(RectPolygon * thiz, float opacity)
{
	thiz->color &= 0x00ffffff;
	thiz->color += (BYTE)(opacity * 0xff) << 24;

	Polygon_SetColor(thiz, thiz->color);
}

float Polygon_GetOpacity(RectPolygon * thiz)
{
	return (float)(thiz->color >> 24)/0xff;
}
