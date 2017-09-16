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
// グローバル変数
//=============================================================================
extern LPDIRECT3DDEVICE9 g_pD3DDevice;
char g_DebugText[20][256] = {};

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void DrawDebug();


//=============================================================================
// 初期化
//=============================================================================
void Renderer::Create(void)
{
	Singleton::Create();

	// レンダリングステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングを行わない
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ縮小時の補間設定

	// テクスチャステージ加算合成処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// ２番目のアルファ引数

	// 初期カメラ
	m_pInstance->camera = &m_pInstance->fixedCamera;


	for (int i = 0; i < (int)Layer::MAX; i++)
	{
		m_pInstance->list[i].reserve(g_PoolSize[i]);
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void Renderer::Destroy(void)
{
	if (m_pInstance == nullptr)
		return;

	for (int i = 0; i < (int)Layer::MAX; i++)
	{
		m_pInstance->list[i].clear();
	}

	Singleton::Destroy();
}

//=============================================================================
// 描画処理
//=============================================================================
void Renderer::drawFrame()
{
	std::vector<RectPolygon*>	list;
	RectPolygon*			poly;

	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), this->camera->backColor, 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		for (int i = 0; i < (int)Layer::MAX; i++)
		{
			list = this->list[i];

			for (size_t j = 0; j < list.size(); j++)
			{
				
				poly = list[j];

				// 頂点座標の更新
				transformVertex(poly);

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
// 描画リストに追加
//=============================================================================
void Renderer::addList(RectPolygon* poly)
{
	poly->listIndex = this->list[(int)poly->layer].size();
	this->list[(int)poly->layer].push_back(poly);
}

//=============================================================================
// 描画リストから削除
//=============================================================================
void Renderer::removeList(RectPolygon * poly)
{
	int index = poly->listIndex;
	std::vector<RectPolygon*>& list = this->list[(int)poly->layer];

	list[index] = list.back();
	list[index]->listIndex = index;
	list.pop_back();

}

Camera * Renderer::getCamera(void)
{
	return this->camera;
}

//=============================================================================
// カメラ設置
//=============================================================================
void Renderer::setCamera(Camera * camera)
{
	if (camera != nullptr)
		this->camera = camera;
	else
		this->camera = &this->fixedCamera;
}


//=============================================================================
// 頂点の座標変換
//=============================================================================
void Renderer::transformVertex(RectPolygon *poly)
{
	// ワールド変換
	Vector3 pos = poly->object->getTransform()->position;
	Vector3 rot = poly->object->getTransform()->rotation;
	Vector3 radius = poly->radius * poly->object->getTransform()->scale;

	poly->vertex[0].vtx.x = pos.x - cosf(poly->baseAngle + rot.z) * radius.x;
	poly->vertex[0].vtx.y = pos.y - sinf(poly->baseAngle + rot.z) * radius.y;
	poly->vertex[0].vtx.z = pos.z;

	poly->vertex[1].vtx.x = pos.x + cosf(poly->baseAngle - rot.z) * radius.x;
	poly->vertex[1].vtx.y = pos.y - sinf(poly->baseAngle - rot.z) * radius.y;
	poly->vertex[1].vtx.z = pos.z;

	poly->vertex[2].vtx.x = pos.x - cosf(poly->baseAngle - rot.z) * radius.x;
	poly->vertex[2].vtx.y = pos.y + sinf(poly->baseAngle - rot.z) * radius.y;
	poly->vertex[2].vtx.z = pos.z;

	poly->vertex[3].vtx.x = pos.x + cosf(poly->baseAngle + rot.z) * radius.x;
	poly->vertex[3].vtx.y = pos.y + sinf(poly->baseAngle + rot.z) * radius.y;
	poly->vertex[3].vtx.z = pos.z;

	if (poly->rendType == RendererType::Default)
	{

		// カメラ変換
		poly->vertex[0].vtx -= this->camera->getTransform()->position;
		poly->vertex[1].vtx -= this->camera->getTransform()->position;
		poly->vertex[2].vtx -= this->camera->getTransform()->position;
		poly->vertex[3].vtx -= this->camera->getTransform()->position;

		// 投影変換
		float fov = Lerpf(poly->vertex[0].vtx.z, 1.0f, this->camera->fov);
		poly->vertex[0].vtx.x /= poly->vertex[0].vtx.z / fov;
		poly->vertex[0].vtx.y /= poly->vertex[0].vtx.z / fov;
		poly->vertex[0].vtx.z = (poly->vertex[0].vtx.z - zmin) / (zmax - zmin);

		poly->vertex[1].vtx.x /= poly->vertex[1].vtx.z / fov;
		poly->vertex[1].vtx.y /= poly->vertex[1].vtx.z / fov;
		poly->vertex[1].vtx.z = (poly->vertex[1].vtx.z - zmin) / (zmax - zmin);

		poly->vertex[2].vtx.x /= poly->vertex[2].vtx.z / fov;
		poly->vertex[2].vtx.y /= poly->vertex[2].vtx.z / fov;
		poly->vertex[2].vtx.z = (poly->vertex[2].vtx.z - zmin) / (zmax - zmin);

		poly->vertex[3].vtx.x /= poly->vertex[3].vtx.z / fov;
		poly->vertex[3].vtx.y /= poly->vertex[3].vtx.z / fov;
		poly->vertex[3].vtx.z = (poly->vertex[3].vtx.z - zmin) / (zmax - zmin);

	}

	// スクリーン変換
	poly->vertex[0].vtx += Vector3(SCREEN_CENTER_X + 0.5f, SCREEN_CENTER_Y + 0.5f, 0.0f);
	poly->vertex[1].vtx += Vector3(SCREEN_CENTER_X + 0.5f, SCREEN_CENTER_Y + 0.5f, 0.0f);
	poly->vertex[2].vtx += Vector3(SCREEN_CENTER_X + 0.5f, SCREEN_CENTER_Y + 0.5f, 0.0f);
	poly->vertex[3].vtx += Vector3(SCREEN_CENTER_X + 0.5f, SCREEN_CENTER_Y + 0.5f, 0.0f);
}

//=============================================================================
// デバッグ文字列の取得
//=============================================================================
char *GetDebugText(int line)
{
	return g_DebugText[line];
}

//=============================================================================
// デバッグ文字の描画
//=============================================================================
void DrawDebug()
{
	RECT rect;
	for (int i = 0; i < 20; i++)
	{
		rect = { 0,i*20,SCREEN_WIDTH,SCREEN_HEIGHT };
		GetFont()->DrawText(NULL, g_DebugText[i], -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}
