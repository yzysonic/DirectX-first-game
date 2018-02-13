#include "GaussianBlur.h"

GaussianBlur::GaussianBlur(Texture* tex_source, RenderTarget* target) : RectPolygon2D(tex_source, Layer::UI_00, "post")
{
	this->pshader = PixelShader::Get("GaussianBlurPS");
	this->strength = 1.0f;
	this->target = target;

	this->vertex[0].pos = Vector3( 0, 0, 0);
	this->vertex[1].pos = Vector3(this->size.x, 0, 0);
	this->vertex[2].pos = Vector3( 0, this->size.y, 0);
	this->vertex[3].pos = Vector3(this->size.x, this->size.y, 0);
}

void GaussianBlur::Draw(void)
{
	auto pDevice = Direct3D::GetDevice();

	// 現在レンダーターゲットの保存
	LPDIRECT3DSURFACE9 pSurface;
	LPDIRECT3DSURFACE9 pDepthSurface;

	pDevice->GetRenderTarget(0, &pSurface);
	pDevice->GetDepthStencilSurface(&pDepthSurface);

	// シェーダーの設定
	pDevice->SetPixelShader(this->pshader->pD3DShader);
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ウェイト計算
	CalculateWeight();
	this->pshader->SetFloatArray("weight", this->weight, WeightSize);

	// パス１
	pDevice->SetRenderTarget(0, this->buffer.pSurface);
	pDevice->SetDepthStencilSurface(this->buffer.pDepthSurface);

	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(125, 125, 125, 1), 1.0f, 0);

	this->pshader->SetBool("is_horizon", true);
	pDevice->SetTexture(0, this->pTexture->pDXTex);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, this->vertex, sizeof(Vertex2D));

	// パス２
	pDevice->SetRenderTarget(0, this->target->pSurface);
	pDevice->SetDepthStencilSurface(this->target->pDepthSurface);

	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(0, 0, 0, 1), 1.0f, 0);

	this->pshader->SetBool("is_horizon", false);
	pDevice->SetTexture(0, this->buffer.pDXTex);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, this->vertex, sizeof(Vertex2D));


	// 元の設定に戻る
	pDevice->SetPixelShader(NULL);
	pDevice->SetRenderTarget(0, pSurface);
	pDevice->SetDepthStencilSurface(pDepthSurface);

	SafeRelease(pSurface);
	SafeRelease(pDepthSurface);
}

bool GaussianBlur::SetActive(bool value)
{
	if(!Drawable::SetActive(value))
		return false;

	if (value)
	{

	}
	else
	{

	}

	return true;
}

void GaussianBlur::CalculateWeight(void)
{
	if (this->strength == 0.0f)
	{
		for (int i = 0; i < WeightSize; i++) 
			this->weight[i] = 0.0f;

		return;
	}

	float t = 0.0;
	float d = this->strength * this->strength / 100.0f;
	for (int i = 0; i < WeightSize; i++) {
		float r = 1.0f + 2.0f * i;
		float w = expf(-0.5f * (r * r) / d);
		this->weight[i] = w;
		if (i > 0) { w *= 2.0f; }
		t += w;
	}
	for (int i = 0; i < WeightSize; i++) {
		this->weight[i] /= t;
	}
}
