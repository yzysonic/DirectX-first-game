#ifndef _POLYGON_H_
#define _POLYGON_H_

#include"main.h"
#include"Texture.h"
#include"Layer.h"

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
	Object *object;
	Vertex2D vertex[RECT_NUM_VERTEX];		// 頂点情報格納ワーク
	Texture *pTexture;						// テクスチャーへのポインタ
	Vector2 size;							// 表示するサイズ
	int pattern;							// 表示するパターン
	Layer layer;							// 描画のレイヤー
	int poolIndex;							// 識別番号
}RectPolygon;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
RectPolygon* newPolygon(Object* object, Layer layer, TextureName texName = TEX_NONE);
void deletePolygon(RectPolygon* thiz);
void Polygon_UpdateVertex(RectPolygon *thiz);


#endif
