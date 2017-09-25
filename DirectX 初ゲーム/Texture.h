#pragma once
#include "main.h"
#include "Direct3D.h"
#include "Vector.h"

#define TEX_DIR "Data/Texture/"

enum TextureName
{
	TEX_NONE,
	TEX_VIGNETTING,
	TEX_TITLE_LOGO,
	TEX_TITLE_PRESSKEY,
	TEX_TITLE_INFO,
	TEX_TITLE_CURSOR,
	TEX_TITLE_START,
	TEX_TITLE_EXIT,
	TEX_NUMBER,
	TEX_GAME_SCORE,
	TEX_GAME_TIME,
	TEX_GAME_OVER,
	TEX_CLEAR,
	TEX_LIFES,
	TEX_PLAYER,
	TEX_ENEMY,
	TEX_BULLET,
	TEX_BULLET_E,
	TEX_MAX
};

typedef struct _Texture
{
	LPDIRECT3DTEXTURE9 pDXTex;	// テクスチャポインタ
	Vector2 size;				// テクスチャサイズ
	int divideX;				// テクスチャ内X分割数
	int divideY;				// テクスチャ内Y分割数
}Texture;

void InitTexture();
void UninitTexture();
Texture *GetTexture(TextureName texName);