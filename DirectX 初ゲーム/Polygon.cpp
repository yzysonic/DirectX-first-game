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

	Polygon_UpdateVertex(polygon);

	// rhwの設定
	polygon->vertex[0].rhw =
	polygon->vertex[1].rhw =
	polygon->vertex[2].rhw =
	polygon->vertex[3].rhw = 1.0f;

	// 反射光の設定
	polygon->vertex[0].diffuse =
	polygon->vertex[1].diffuse =
	polygon->vertex[2].diffuse =
	polygon->vertex[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

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