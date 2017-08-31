#include "Renderer.h"
#include "RendererType.h"
#include "Direct3D.h"
#include "SceneGame.h"
#include "Time.h"

const int g_PoolSize = ObjectMax / LAYER_MAX;

typedef struct _PolygonPool
{
	RectPolygon polygon[g_PoolSize];
	int activeTop = -1;
}PolygonPool;

void DrawPoly(RectPolygon *poly);
void DrawTexPoly(RectPolygon *poly);

extern LPDIRECT3DDEVICE9	g_pD3DDevice;
PolygonPool					g_PolygonPool[LAYER_MAX];


//=============================================================================
// メイン描画処理
//=============================================================================
void DrawFrame()
{
	PolygonPool* pool;
	RectPolygon* poly;

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

				switch (poly->rendererType)
				{
				case REND_POLY:
					DrawPoly(poly);
					break;
				case REND_TEX_POLY:
					DrawTexPoly(poly);
					break;
				}
			}
		}

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

RectPolygon * Renderer_GetPolygon(Layer layer)
{
	PolygonPool* pool = &g_PolygonPool[layer];

	if (pool->activeTop < g_PoolSize - 1)
	{
		RectPolygon* thiz = &pool->polygon[++pool->activeTop];
		thiz->layer = layer;
		thiz->poolIndex = pool->activeTop;
		return thiz;
	}
	else
		return NULL;
}

void Renderer_ReleasePolygon(RectPolygon * thiz)
{
	PolygonPool* pool = &g_PolygonPool[thiz->layer];
	pool->polygon[thiz->poolIndex] = pool->polygon[pool->activeTop--];
}


//=============================================================================
// レンダラー別の描画処理
//=============================================================================
void DrawPoly(RectPolygon * poly)
{
	// 頂点フォーマットの設定
	g_pD3DDevice->SetFVF(FVF_VERTEX_2D_NOTEX);

	// 頂点座標の更新
	Polygon_UpdateVertex(poly);

	// ポリゴンの描画
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, poly->vertex, sizeof(Vertex2D) - sizeof(Vector2));
}
void DrawTexPoly(RectPolygon * poly)
{
	// 頂点フォーマットの設定
	g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点座標の更新
	Polygon_UpdateVertex(poly);

	// テクスチャの設定
	g_pD3DDevice->SetTexture(0, poly->pTexture->pDXTex);

	// ポリゴンの描画
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, poly->vertex, sizeof(Vertex2D));

}
