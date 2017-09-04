#pragma once
#include"main.h"

#define TEX_DIR "Data/Texture/"

enum TextureName
{
	TEX_NONE,
	TEX_PLAYER,
	TEX_MAX
};

typedef struct _Texture
{
	LPDIRECT3DTEXTURE9 pDXTex;	// テクスチャポインタ
	Vector2 size;				// テクスチャサイズ
	Vector2 divide;				// テクスチャ内分割数
}Texture;

void InitTexture();
void UninitTexture();
Texture *GetTexture(TextureName texName);