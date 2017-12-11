#include "Renderer.h"
#include "Direct3D.h"
#include "RenderSpace.h"


//=============================================================================
// グローバル変数
//=============================================================================
char g_DebugText[40][256] = {};

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

	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();

	// レンダリングステートパラメータの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// カリングを行わない
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);					// Zバッファを使用
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャステージ加算合成処理
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// ２番目のアルファ引数

	// 初期カメラ
	//m_pInstance->camera = &m_pInstance->fixedCamera;


	//for (int i = 0; i < (int)Layer::MAX; i++)
	//{
	//	m_pInstance->list[i].reserve(g_PoolSize[i]);
	//}

	RenderSpace::Add("default");

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
		//m_pInstance->list[i].clear();
	}

	for (int i = RenderSpace::RenderSpaceCount(); i >=0; i--)
		RenderSpace::Delete(i);

	Singleton::Destroy();
}

//=============================================================================
// 描画処理
//=============================================================================
void Renderer::DrawFrame()
{
	auto pDevice = Direct3D::GetDevice();

	// Direct3Dによる描画の開始
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		RenderSpace::Draw();

		// デバッグ
		DrawDebug();

		// Direct3Dによる描画の終了
		pDevice->EndScene();
	}

	pDevice->SetRenderTarget(0, RenderTarget::BackBuffer()->pSurface);
	pDevice->SetDepthStencilSurface(RenderTarget::BackBuffer()->pDepthSurface);

	// バックバッファとフロントバッファの入れ替え
	static HRESULT hr;
	hr = pDevice->Present(NULL, NULL, NULL, NULL);

	// デバイスロストの検知
	if (hr == D3DERR_DEVICELOST) {

		// 復帰可能の場合
		if (pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			Direct3D::ResetDevice();
		}
	}
}

//=============================================================================
// 描画リストに追加
//=============================================================================
//void Renderer::addList(Drawable* poly)
//{
//	poly->list_index = this->list[(int)poly->layer].size();
//	this->list[(int)poly->layer].push_back(poly);
//}

//=============================================================================
// 描画リストから削除
//=============================================================================
//void Renderer::removeList(Drawable * poly)
//{
//	int index = poly->list_index;
//	std::vector<Drawable*>& list = this->list[(int)poly->layer];
//
//	list[index] = list.back();
//	list[index]->list_index = index;
//	list.pop_back();
//
//}

Camera * Renderer::getCamera(void)
{
	return RenderSpace::Get(0)->GetCamera(0);
}

//=============================================================================
// カメラ設置
//=============================================================================
void Renderer::setCamera(Camera * camera)
{
	RenderSpace::Get(0)->RemoveCamera(RenderSpace::Get(0)->GetCamera(0));

	if (camera != nullptr)
	{
		RenderSpace::Get(0)->AddCamera(camera);
	}
	else
		RenderSpace::Get(0)->AddCamera(&fixedCamera);
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
	for (int i = 0; i < 40; i++)
	{
		rect = { 0,i * 20,SystemParameters::ResolutionX,SystemParameters::ResolutionY };
		Direct3D::GetFont()->DrawText(NULL, g_DebugText[i], -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}
