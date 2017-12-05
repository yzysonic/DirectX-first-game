#include "Renderer.h"
#include "Direct3D.h"


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

	// レンダリングステートパラメータの設定
	Direct3D::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// カリングを行わない
	Direct3D::GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	Direct3D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	Direct3D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	Direct3D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	Direct3D::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	Direct3D::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	Direct3D::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	Direct3D::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャステージ加算合成処理
	Direct3D::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	Direct3D::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	Direct3D::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// ２番目のアルファ引数

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
void Renderer::DrawFrame()
{
	std::vector<Drawable*>	list;
	Drawable*	poly;

	// バックバッファ＆Ｚバッファのクリア
	Direct3D::GetDevice()->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), m_pInstance->camera->getD3DBackColor(), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(Direct3D::GetDevice()->BeginScene()))
	{

		Camera* camera = m_pInstance->camera;
		camera->getViewMatrix(true);
		camera->getProjectionMatrix(true);

		for (int i = 0; i < (int)Layer::MAX; i++)
		{
			list = m_pInstance->list[i];

			for (size_t j = 0; j < list.size(); j++)
			{
				poly = list[j];
				poly->draw();
			}
		}

		// デバッグ
		DrawDebug();

		// Direct3Dによる描画の終了
		Direct3D::GetDevice()->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	static HRESULT hr;
	hr = Direct3D::GetDevice()->Present(NULL, NULL, NULL, NULL);

	// デバイスロストの検知
	if (hr == D3DERR_DEVICELOST) {

		// 復帰可能の場合
		if (Direct3D::GetDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			Direct3D::ResetDevice();
		}
	}
}

//=============================================================================
// 描画リストに追加
//=============================================================================
void Renderer::addList(Drawable* poly)
{
	poly->list_index = this->list[(int)poly->layer].size();
	this->list[(int)poly->layer].push_back(poly);
}

//=============================================================================
// 描画リストから削除
//=============================================================================
void Renderer::removeList(Drawable * poly)
{
	int index = poly->list_index;
	std::vector<Drawable*>& list = this->list[(int)poly->layer];

	list[index] = list.back();
	list[index]->list_index = index;
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
