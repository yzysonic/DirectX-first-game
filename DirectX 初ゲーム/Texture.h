#pragma once
#include"main.h"

#define TEX_DIR "Data/Texture/"

typedef enum
{
	TEX_NONE = -1,
	TEX_PLAYER,
	TEX_MAX
}TextureName;

typedef struct _Texture
{
	LPDIRECT3DTEXTURE9 pDXTex;	// �e�N�X�`���|�C���^
	Vector2 size;				// �e�N�X�`���T�C�Y
	Vector2 divide;				// �e�N�X�`����������
}Texture;

void InitTexture();
void UninitTexture();
Texture *GetTexture(TextureName texName);