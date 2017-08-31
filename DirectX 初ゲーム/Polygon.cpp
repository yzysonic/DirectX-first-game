#include "Polygon.h"
#include "Object.h"
#include "Renderer.h"


RectPolygon* newPolygon(Object* object, Layer layer, TextureName texName, RendererType rendType)
{
	RectPolygon *polygon = Renderer_GetPolygon(layer);

	polygon->object = object;

	// rhwの設定
	polygon->vertex[0].rhw =
	polygon->vertex[1].rhw =
	polygon->vertex[2].rhw =
	polygon->vertex[3].rhw = 1.0f;

	// 反射光の設定
	polygon->vertex[0].diffuse =
	polygon->vertex[1].diffuse =
	polygon->vertex[2].diffuse =
	polygon->vertex[3].diffuse = ColorRGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	polygon->vertex[0].uv = Vector2(0.0f, 0.0f);
	polygon->vertex[1].uv = Vector2(1.0f, 0.0f);
	polygon->vertex[2].uv = Vector2(0.0f, 1.0f);
	polygon->vertex[3].uv = Vector2(1.0f, 1.0f);

	polygon->rendererType = rendType;

	Polygon_SetTexture(polygon, texName);
	Polygon_UpdateVertex(polygon);

	polygon->pattern = 0;

	return polygon;
}

void deletePolygon(RectPolygon * thiz)
{
	thiz->object = NULL;
	thiz->pTexture = NULL;
	Renderer_ReleasePolygon(thiz);
}

void Polygon_UpdateVertex(RectPolygon *thiz)
{
	Vector3 pos = thiz->object->transform->position;

	thiz->vertex[0].vtx = pos + Vector3(-thiz->size.x / 2, -thiz->size.y / 2, 0.0f);
	thiz->vertex[1].vtx = pos + Vector3(+thiz->size.x / 2, -thiz->size.y / 2, 0.0f);
	thiz->vertex[2].vtx = pos + Vector3(-thiz->size.x / 2, +thiz->size.y / 2, 0.0f);
	thiz->vertex[3].vtx = pos + Vector3(+thiz->size.x / 2, +thiz->size.y / 2, 0.0f);

}

void Polygon_SetTexture(RectPolygon * thiz, TextureName texName)
{
	thiz->pTexture = GetTexture(texName);
	if (thiz->pTexture)
		thiz->size = thiz->pTexture->size;
	else
	{
		Polygon_SetColor(thiz, ColorRGBA(255, 21, 243, 255));
		thiz->rendererType = REND_POLY;
		thiz->size = Vector2(100.f, 100.f);
	}
		
}


void Polygon_SetColor(RectPolygon * thiz, D3DCOLOR color)
{
	for (int i = 0; i < RECT_NUM_VERTEX; i++)
		thiz->vertex[i].diffuse = color;
}
