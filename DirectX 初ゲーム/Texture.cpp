#include "Texture.h"
#include "Direct3D.h"


Texture g_textureList[TEX_MAX];

void LoadTexture(TextureName texName, LPSTR fileName, Vector2 divide = Vector2(1.f, 1.f));

void InitTexture()
{
	LoadTexture(TEX_PLAYER, "player.png");

}

void UninitTexture()
{
	for (int i = 0; i < TEX_MAX; i++)
		SafeRelease(g_textureList[i].pDXTex);
}

Texture * GetTexture(TextureName texName)
{
	if (texName != TEX_NONE)
		return &g_textureList[texName];
	else
		return NULL;
}


void LoadTexture(TextureName texName, LPSTR fileName, Vector2 divide)
{
	Texture &texture = g_textureList[texName];

	char fileDir[256];
	strcat(strcpy(fileDir, TEX_DIR), fileName);

	//テクスチャ読込
	D3DXCreateTextureFromFile(GetDevice(), fileDir, &texture.pDXTex);
	if (texture.pDXTex == NULL)
	{
		TCHAR s[128];
		wsprintf(s, _T("テクスチャー「%s」の読込に失敗しました。"), fileName);
		MessageBox(GetHWnd(), s, _T("エラー"), MB_OK | MB_ICONWARNING);
		return;
	}

	texture.divide = divide;

	//テクスチャ詳細情報取得
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(fileDir, &info);
	texture.size.x = info.Width / divide.x;
	texture.size.y = info.Height / divide.y;
}

