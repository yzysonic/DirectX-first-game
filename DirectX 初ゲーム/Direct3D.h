//=============================================================================
//
// ファイル名：graphics.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <d3dx9.h>


HRESULT InitDirect3D(HWND hWnd, int screenWidth, int screenHeight, bool bWindowMode);
void UninitDirect3D();
LPDIRECT3DDEVICE9 GetDevice();
LPD3DXFONT GetFont();


#endif // _GRAPHICS_H_


