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

	// 初期描画空間
	RenderSpace::Add("default");

	// 描画中カメラの設定
	m_pInstance->fixedCamera = new Camera;
	m_pInstance->drawing_camera = m_pInstance->fixedCamera;
}

//=============================================================================
// 終了処理
//=============================================================================
void Renderer::Destroy(void)
{
	if (m_pInstance == nullptr)
		return;

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
		// メイン描画処理
		RenderSpace::Draw();

		// デバッグ
		DrawDebug();

		// Direct3Dによる描画の終了
		pDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	HRESULT hr;
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
// カメラ取得
//=============================================================================
Camera * Renderer::getCamera(void)
{
	return m_pInstance->drawing_camera;
}
Camera * Renderer::getCamera(std::string render_space_name, int no)
{
	return RenderSpace::Get(render_space_name)->GetCamera(no);
}
Camera * Renderer::getCamera(int render_space_no, int no)
{
	return RenderSpace::Get(render_space_no)->GetCamera(no);
}


//=============================================================================
// カメラ設置
//=============================================================================
void Renderer::setCamera(Camera * camera, std::string render_space)
{
	RenderSpace::Get(render_space)->RemoveCamera(RenderSpace::Get(render_space)->GetCamera(0));

	if (camera != nullptr)
		RenderSpace::Get(render_space)->AddCamera(camera);
	else
		RenderSpace::Get(render_space)->AddCamera(fixedCamera);
}
void Renderer::setCamera(Camera * camera, int render_space)
{
	RenderSpace::Get(render_space)->RemoveCamera(RenderSpace::Get(render_space)->GetCamera(0));

	if (camera != nullptr)
		RenderSpace::Get(render_space)->AddCamera(camera);
	else
		RenderSpace::Get(render_space)->AddCamera(fixedCamera);
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
	auto pDevice = Direct3D::GetDevice();
	pDevice->SetRenderTarget(0, RenderTarget::BackBuffer()->pSurface);
	pDevice->SetDepthStencilSurface(RenderTarget::BackBuffer()->pDepthSurface);

	for (int i = 0; i < 40; i++)
	{
		rect = { 0,i * 20,SystemParameters::ResolutionX,SystemParameters::ResolutionY };
		Direct3D::GetFont()->DrawText(NULL, g_DebugText[i], -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}
