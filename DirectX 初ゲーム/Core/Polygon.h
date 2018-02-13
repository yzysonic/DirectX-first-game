#pragma once

#include "Common.h"
#include "Texture.h"
#include "Drawable.h"
#include "Object.h"
#include "Component.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	RECT_NUM_VERTEX		(4)		// 頂点数
#define	RECT_NUM_POLYGON	(2)		// ポリゴン数

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3Dポリゴン頂点フォーマット( 頂点座標 / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct Vertex2D
{
	Vector3 pos;		// 頂点座標
	float rhw;			// テクスチャのパースペクティブコレクト用
	Color diffuse;		// 反射光
	Vector2 uv;			// テクスチャ座標
};

struct Vertex3D
{
	Vector3 pos;
	Vector3 nor;
	Color diffuse;
	Vector2 uv;
};

class RectPolygon2D : public Drawable
{
public:
	Vertex2D vertex[RECT_NUM_VERTEX];		// 頂点情報格納ワーク
	Texture *pTexture;						// テクスチャーへのポインタ
	float radius;							// 頂点計算用半径
	float baseAngle;						// 頂点計算用角度
	
	RectPolygon2D(Texture* texture, Layer layer = Layer::DEFAULT, std::string render_space_name = "default");
	RectPolygon2D(std::string texture_name = "none", Layer layer = Layer::DEFAULT, std::string render_space_name = "default");
	void Draw(void) override;
	void SetColor(Color color);
	Vector2 getSize(void);
	void setSize(float x, float y);
	void setPattern(unsigned int pattern);
	void SetTexture(std::string texture_name);
	void SetTexture(Texture* texture);

protected:
	Vector2 size;	// 表示するサイズ
	int pattern;	// 表示するパターン

	void transformVertex(void);
};

class RectPolygon : public Drawable, public ILostAndReset
{
public:
	Texture *pTexture;

	RectPolygon(std::string texture_name = "none", Layer layer = Layer::DEFAULT, std::string render_space_name = "default");
	~RectPolygon(void);
	HRESULT OnLostDevice(void);
	HRESULT OnResetDevice(void);
	void Draw(void) override;
	void SetColor(Color color);
	Vector2 GetSize(void);
	void SetSize(Vector2 size);
	void SetTexture(std::string texture_name);
	void SetTexture(Texture* texture);

protected:
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	Vector2 size;

	HRESULT InitBuffer(void);

};
