#include "Texture.h"
#include "Direct3D.h"


Texture g_textureList[TEX_MAX];

void LoadTexture(TextureName texName, LPSTR fileName, Vector2 divide = Vector2(1.f, 1.f));

void InitTexture()
{
	g_textureList[TEX_NONE].pDXTex = NULL;
	g_textureList[TEX_NONE].divide = Vector2(1, 1);
	g_textureList[TEX_NONE].size = Vector2(100, 100);

	LoadTexture(TEX_VIGNETTING,		"vignetting.png");
	LoadTexture(TEX_TITLE_LOGO,		"title_logo.png");
	LoadTexture(TEX_TITLE_PRESSKEY, "title_press_enter.png");
	LoadTexture(TEX_TITLE_INFO,		"title_copyright.png");
	LoadTexture(TEX_PLAYER,			"player.png");
	LoadTexture(TEX_BULLET,			"bullet.png");
}

void UninitTexture()
{
	for (int i = 0; i < TEX_MAX; i++)
		SafeRelease(g_textureList[i].pDXTex);
}

Texture * GetTexture(TextureName texName)
{
	return &g_textureList[texName];
}


void LoadTexture(TextureName texName, LPSTR fileName, Vector2 divide)
{
	Texture &texture = g_textureList[texName];

	char fileDir[256];
	strcat(strcpy(fileDir, TEX_DIR), fileName);

	//テクスチャ詳細情報取得
	D3DXIMAGE_INFO info;
	if (D3DXGetImageInfoFromFile(fileDir, &info) == D3D_OK)
	{
		//テクスチャ読込
		D3DXCreateTextureFromFileEx(
			GetDevice(),
			fileDir,
			info.Width, info.Height,
			1, 0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0xFF000000,
			NULL, NULL,
			&texture.pDXTex);
	}
	else
	{
		TCHAR s[128];
		wsprintf(s, _T("テクスチャー「%s」の読込に失敗しました。"), fileName);
		MessageBox(GetHWnd(), s, _T("エラー"), MB_OK | MB_ICONWARNING);
		return;

	}

	texture.size.x = info.Width / divide.x;
	texture.size.y = info.Height / divide.y;
	texture.divide = divide;

	
}

