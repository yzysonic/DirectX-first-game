#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"
#include "Texture.h"
#include "Layer.h"
#include "RendererType.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	RECT_NUM_VERTEX		(4)		// 頂点数
#define	RECT_NUM_POLYGON	(2)		// ポリゴン数

// 頂点フォーマット
#define	FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_2D_NOTEX	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct _Object Object;

typedef struct _Vertex2DLite
{
	Vector3 vtx;		// 頂点座標
	float rhw;			// テクスチャのパースペクティブコレクト用
	Color diffuse;		// 反射光
} Vertex2DLite;

typedef struct _Vertex2D
{
	Vector3 vtx;		// 頂点座標
	float rhw;			// テクスチャのパースペクティブコレクト用
	Color diffuse;		// 反射光
	Vector2 uv;			// テクスチャ座標
} Vertex2D;


typedef struct _RectPolygon
{
	Object *object;
	Vertex2D vertex[RECT_NUM_VERTEX];		// 頂点情報格納ワーク
	Texture *pTexture;						// テクスチャーへのポインタ
	Vector2 size;							// 表示するサイズ
	int pattern;							// 表示するパターン
	Layer layer;							// 描画のレイヤー
	int poolIndex;							// 識別番号
	//void(*draw)(RectPolygon* thiz);			// 描画処理
	RendererType rendererType;				// 描画用レンダラータイプ
}RectPolygon;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
RectPolygon* newPolygon(Object* object, Layer layer = LAYER_DEFAULT, TextureName texName = TEX_NONE, RendererType rendType = REND_POLY);
void deletePolygon(RectPolygon* thiz);
void Polygon_UpdateVertex(RectPolygon *thiz);
void Polygon_SetTexture(RectPolygon *thiz, TextureName texName);
void Polygon_SetColor(RectPolygon *thiz, D3DCOLOR color);


#endif
