#include "Renderer.h"
#include "Direct3D.h"
#include "Time.h"
#include "Lerp.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define zmax 100.0f
#define zmin 0.1f

//=============================================================================
// 構造体宣言
//=============================================================================
typedef struct _PolygonPool
{
	RectPolygon *polygon = NULL;
	int activeTop = -1;
}PolygonPool;

//=============================================================================
// グローバル変数
//=============================================================================
extern LPDIRECT3DDEVICE9 g_pD3DDevice;
PolygonPool	g_PolygonPool[LAYER_MAX];
Transform g_FixedCamera;
Transform *g_Camera;
int g_PoolSize[LAYER_MAX];
char g_DebugText[10][256] = {};
float g_fov;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void TransformVertex(RectPolygon *thiz);
void DrawDebug();


//=============================================================================
// 初期化
//=============================================================================
void InitRenderer(void)
{
	// レンダリングステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングを行わない
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);		// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);		// テクスチャ縮小時の補間設定

	// テクスチャステージ加算合成処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// ２番目のアルファ引数

	// 初期カメラ
	g_Camera = &g_FixedCamera;
	g_fov = 0.0f;

	// ポリゴンが使うメモリの確保
	if (g_PolygonPool[0].polygon != NULL)
		return;

	g_PoolSize[LAYER_BG_00]		= POOL_SIZE_BG_00;
	g_PoolSize[LAYER_BG_01]		= POOL_SIZE_BG_01;
	g_PoolSize[LAYER_BG_02]		= POOL_SIZE_BG_02;
	g_PoolSize[LAYER_DEFAULT]	= POOL_SIZE_DEFAULT;
	g_PoolSize[LAYER_PLAYER]	= POOL_SIZE_PLAYER;
	g_PoolSize[LAYER_UI_00]		= POOL_SIZE_UI_00;
	g_PoolSize[LAYER_UI_01]		= POOL_SIZE_UI_01;
	g_PoolSize[LAYER_UI_02]		= POOL_SIZE_UI_02;

	for (int i = 0; i < LAYER_MAX; i++)
	{
		g_PolygonPool[i].polygon = (RectPolygon*)malloc(sizeof(RectPolygon)*g_PoolSize[i]);
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRenderer(void)
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		SafeDelete(g_PolygonPool[i].polygon);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFrame()
{
	PolygonPool*		pool;
	RectPolygon*		poly;

	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(200, 200, 200, 255), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		for (int i = 0; i < LAYER_MAX; i++)
		{
			pool = &g_PolygonPool[i];

			for (int j = 0; j <= pool->activeTop; j++)
			{
				
				poly = &pool->polygon[j];

				// 頂点座標の更新
				TransformVertex(poly);

				// 頂点フォーマットの設定
				g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

				// テクスチャの設定
				g_pD3DDevice->SetTexture(0, poly->pTexture->pDXTex);

				// ポリゴンの描画
				g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, poly->vertex, sizeof(Vertex2D));
			}
		}

		// デバッグ
		DrawDebug();

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	static HRESULT hr;
	hr = g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	// デバイスロストの検知
	if (hr == D3DERR_DEVICELOST) {

		// 復帰可能の場合
		if (g_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {

			ResetDevice(GetWindowMode());
		}
	}
}

//=============================================================================
// ポリゴン資源の取得
//=============================================================================
RectPolygon * Renderer_GetPolygon(Layer layer)
{
	PolygonPool* pool = &g_PolygonPool[layer];

	if (pool->activeTop < g_PoolSize[layer] - 1)
	{
		RectPolygon* thiz = &pool->polygon[++pool->activeTop];
		thiz->layer = layer;
		thiz->poolIndex = pool->activeTop;
		return thiz;
	}
	else
		return NULL;
}

//=============================================================================
// ポリゴン資源の釈放
//=============================================================================
void Renderer_ReleasePolygon(RectPolygon * thiz)
{
	PolygonPool* pool = &g_PolygonPool[thiz->layer];
	pool->polygon[thiz->poolIndex] = pool->polygon[pool->activeTop--];
}

//=============================================================================
// カメラ設置
//=============================================================================
void Renderer_SetCamera(Transform * camera)
{
	if (camera != NULL)
		g_Camera = camera;
	else
		g_Camera = &g_FixedCamera;
}

//=============================================================================
// 頂点の座標変換
//=============================================================================
void TransformVertex(RectPolygon *thiz)
{
	// ワールド変換
	Vector3 pos = thiz->object->transform->position;
	Vector3 rot = thiz->object->transform->rotation;
	Vector3 radius = thiz->radius * thiz->object->transform->scale;

	thiz->vertex[0].vtx.x = pos.x - cosf(thiz->baseAngle + rot.z) * radius.x;
	thiz->vertex[0].vtx.y = pos.y - sinf(thiz->baseAngle + rot.z) * radius.y;
	thiz->vertex[0].vtx.z = pos.z;

	thiz->vertex[1].vtx.x = pos.x + cosf(thiz->baseAngle - rot.z) * radius.x;
	thiz->vertex[1].vtx.y = pos.y - sinf(thiz->baseAngle - rot.z) * radius.y;
	thiz->vertex[1].vtx.z = pos.z;

	thiz->vertex[2].vtx.x = pos.x - cosf(thiz->baseAngle - rot.z) * radius.x;
	thiz->vertex[2].vtx.y = pos.y + sinf(thiz->baseAngle - rot.z) * radius.y;
	thiz->vertex[2].vtx.z = pos.z;

	thiz->vertex[3].vtx.x = pos.x + cosf(thiz->baseAngle + rot.z) * radius.x;
	thiz->vertex[3].vtx.y = pos.y + sinf(thiz->baseAngle + rot.z) * radius.y;
	thiz->vertex[3].vtx.z = pos.z;

	// カメラ変換
	thiz->vertex[0].vtx -= g_Camera->position;
	thiz->vertex[1].vtx -= g_Camera->position;
	thiz->vertex[2].vtx -= g_Camera->position;
	thiz->vertex[3].vtx -= g_Camera->position;

	// 投影変換
	float fov = Lerpf(thiz->vertex[0].vtx.z, 1.0f, g_fov);
	thiz->vertex[0].vtx.x /= thiz->vertex[0].vtx.z / fov;
	thiz->vertex[0].vtx.y /= thiz->vertex[0].vtx.z / fov;
	thiz->vertex[0].vtx.z = (thiz->vertex[0].vtx.z - zmin) / (zmax - zmin);

	thiz->vertex[1].vtx.x /= thiz->vertex[1].vtx.z / fov;
	thiz->vertex[1].vtx.y /= thiz->vertex[1].vtx.z / fov;
	thiz->vertex[1].vtx.z = (thiz->vertex[1].vtx.z - zmin) / (zmax - zmin);

	thiz->vertex[2].vtx.x /= thiz->vertex[2].vtx.z / fov;
	thiz->vertex[2].vtx.y /= thiz->vertex[2].vtx.z / fov;
	thiz->vertex[2].vtx.z = (thiz->vertex[2].vtx.z - zmin) / (zmax - zmin);

	thiz->vertex[3].vtx.x /= thiz->vertex[3].vtx.z / fov;
	thiz->vertex[3].vtx.y /= thiz->vertex[3].vtx.z / fov;
	thiz->vertex[3].vtx.z = (thiz->vertex[3].vtx.z - zmin) / (zmax - zmin);

	// スクリーン変換
	thiz->vertex[0].vtx += Vector3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	thiz->vertex[1].vtx += Vector3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	thiz->vertex[2].vtx += Vector3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	thiz->vertex[3].vtx += Vector3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
}

//=============================================================================
// デバッグ文字列の取得
//=============================================================================
char *GetDebugText(int line)
{
	return g_DebugText[line];
}

float Renderer_GetFov()
{
	return g_fov;
}

void Renderer_SetFov(float value)
{
	g_fov = value;
}

//=============================================================================
// デバッグ文字の描画
//=============================================================================
void DrawDebug()
{
	RECT rect;
	for (int i = 0; i < 10; i++)
	{
		rect = { 0,i*20,SCREEN_WIDTH,SCREEN_HEIGHT };
		GetFont()->DrawText(NULL, g_DebugText[i], -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}
