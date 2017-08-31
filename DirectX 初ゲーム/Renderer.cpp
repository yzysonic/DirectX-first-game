#include "Renderer.h"
#include "Direct3D.h"
#include "SceneGame.h"
#include "Time.h"

const int g_PoolSize = ObjectMax / LAYER_MAX;

typedef struct _PolygonPool
{
	RectPolygon polygon[g_PoolSize];
	int activeTop = -1;
}PolygonPool;


PolygonPool	g_PolygonPool[LAYER_MAX];



//=============================================================================
// 描画処理
//=============================================================================
void DrawFrame()
{
	LPDIRECT3DDEVICE9	device = GetDevice();
	PolygonPool*		pool;
	RectPolygon*		poly;

	// バックバッファ＆Ｚバッファのクリア
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(200, 200, 200, 255), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(device->BeginScene()))
	{
		// 頂点フォーマットの設定
		device->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < LAYER_MAX; i++)
		{
			pool = &g_PolygonPool[i];

			for (int j = 0; j <= pool->activeTop; j++)
			{
				
				poly = &pool->polygon[j];

				// 頂点座標の更新
				Polygon_UpdateVertex(poly);
				device->SetFVF(FVF_VERTEX_2D);
				// テクスチャの設定
				device->SetTexture(0, poly->pTexture->pDXTex);

				// ポリゴンの描画
				device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, poly->vertex, sizeof(Vertex2D));
			}
		}

		// Direct3Dによる描画の終了
		device->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	device->Present(NULL, NULL, NULL, NULL);
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
