//=============================================================================
//
// ファイル名：graphics.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "d3dx9.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")

#define	NUM_VERTEX	(4)		// 頂点数
#define	NUM_POLYGON	(2)		// ポリゴン数

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 上記頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標(*33)
} VERTEX_2D;

HRESULT InitGraphics(HWND hWnd, int screenWidth, int screenHeight, bool bWindowMode);
void UninitGraphics();
void DrawFrame();



#endif // _GRAPHICS_H_


