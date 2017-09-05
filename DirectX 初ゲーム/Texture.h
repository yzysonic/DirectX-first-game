#pragma once
#include"main.h"

#define TEX_DIR "Data/Texture/"

enum TextureName
{
	TEX_NONE,
	TEX_TITLE_LOGO,
	TEX_TITLE_PRESSKEY,
	TEX_TITLE_INFO,
	TEX_PLAYER,
	TEX_BULLET,
	TEX_MAX
};

typedef struct _Texture
{
	LPDIRECT3DTEXTURE9 pDXTex;	// �e�N�X�`���|�C���^
	Vector2 size;				// �e�N�X�`���T�C�Y
	Vector2 divide;				// �e�N�X�`����������
}Texture;

void InitTexture();
void UninitTexture();
Texture *GetTexture(TextureName texName);