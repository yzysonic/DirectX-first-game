#include "Texture.h"
#include "Direct3D.h"


Texture g_textureList[TEX_MAX];

void LoadTexture(TextureName texName, LPSTR fileName, int divX = 1, int divY = 1);

void InitTexture()
{
	g_textureList[TEX_NONE].pDXTex = NULL;
	g_textureList[TEX_NONE].divideX = 1;
	g_textureList[TEX_NONE].divideY = 1;
	g_textureList[TEX_NONE].size = Vector2(100, 100);

	LoadTexture(TEX_VIGNETTING,		"vignetting.png");
	LoadTexture(TEX_TITLE_LOGO,		"title_logo.png");
	LoadTexture(TEX_TITLE_PRESSKEY, "title_press_enter.png");
	LoadTexture(TEX_TITLE_INFO,		"title_copyright.png");
	LoadTexture(TEX_TITLE_CURSOR,	"title_cursor_v2.png");
	LoadTexture(TEX_TITLE_START,	"title_start.png");
	LoadTexture(TEX_TITLE_EXIT,		"title_exit.png");
	LoadTexture(TEX_NUMBER,			"number.png", 10);
	LoadTexture(TEX_GAME_SCORE,		"game_score.png");
	LoadTexture(TEX_GAME_TIME,		"game_time.png");
	LoadTexture(TEX_GAME_OVER,		"game_over.png");
	LoadTexture(TEX_CLEAR,			"clear.png");
	LoadTexture(TEX_LIFES,			"lives.png", 3);
	LoadTexture(TEX_PLAYER,			"player.png");
	LoadTexture(TEX_ENEMY,			"enemy.png");
	LoadTexture(TEX_BULLET,			"bullet.png");
	LoadTexture(TEX_BULLET_E,		"bullet_e.png");
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


void LoadTexture(TextureName texName, LPSTR fileName, int divX, int divY)
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

	texture.size.x = (float)info.Width / divX;
	texture.size.y = (float)info.Height / divY;
	texture.divideX = divX;
	texture.divideY = divY;

	
}

