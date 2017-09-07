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

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct _Object Object;

typedef struct _VERTEX_2D
{
	Vector3 vtx;		// 頂点座標
	float rhw;			// テクスチャのパースペクティブコレクト用
	Color diffuse;		// 反射光
	Vector2 uv;			// テクスチャ座標
} Vertex2D;

typedef struct _RectPolygon
{
	Object *object;							// 所有するオブジェクトへの参照
	RendererType rendType;					// 描画方法指定
	Vertex2D vertex[RECT_NUM_VERTEX];		// 頂点情報格納ワーク
	Texture *pTexture;						// テクスチャーへのポインタ
	Vector2 size;							// 表示するサイズ
	int pattern;							// 表示するパターン
	Layer layer;							// 描画のレイヤー
	int poolIndex;							// 識別番号
	float radius;							// 頂点計算用半径
	float baseAngle;						// 頂点計算用角度
	Color color;							// 色（読込専用）
}RectPolygon;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
RectPolygon* newPolygon(Object* object, Layer layer = LAYER_DEFAULT, TextureName texName = TEX_NONE, RendererType rendType = REND_DEFAULT);
void deletePolygon(RectPolygon* thiz);
void Polygon_SetSize(RectPolygon* thiz, float x, float y);
void Polygon_SetColor(RectPolygon *thiz, D3DCOLOR color);
void Polygon_SetOpacity(RectPolygon *thiz, float opacity);
float Polygon_GetOpacity(RectPolygon *thiz);

#endif
