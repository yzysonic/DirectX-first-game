#pragma once

#include "Common.h"
#include "Texture.h"
#include "Layer.h"
#include "RendererType.h"
#include "Color.h"

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
class Object;

struct Vertex2D
{
	Vector3 vtx;		// 頂点座標
	float rhw;			// テクスチャのパースペクティブコレクト用
	Color diffuse;		// 反射光
	Vector2 uv;			// テクスチャ座標
};

class RectPolygon
{
public:
	Object *object;							// 所有するオブジェクトへの参照
	RendererType rendType;					// 描画方法指定
	Vertex2D vertex[RECT_NUM_VERTEX];		// 頂点情報格納ワーク
	Texture *pTexture;						// テクスチャーへのポインタ
	int listIndex;							// 識別番号
	float radius;							// 頂点計算用半径
	float baseAngle;						// 頂点計算用角度
	
	RectPolygon(Object* object, Layer layer, TextureName texName, RendererType rendType);
	~RectPolygon(void);
	Layer getLayer(void);
	Vector2 getSize(void);
	void setSize(float x, float y);
	Color getColor(void);
	void setColor(Color color);
	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	float getOpacity(void);
	void setOpacity(float value);
	void setPattern(unsigned int pattern);

protected:
	Layer layer;							// 描画のレイヤー
	Vector2 size;							// 表示するサイズ
	Color color;							// 色
	int pattern;							// 表示するパターン

	friend class Renderer;
};

