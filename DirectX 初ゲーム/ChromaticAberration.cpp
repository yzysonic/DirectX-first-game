#include "ChromaticAberration.h"

ChromaticAberration::ChromaticAberration(Texture * tex_source, RenderTarget * target) : RectPolygon2D(tex_source, Layer::UI_01, "post")
{
	this->pshader = PixelShader::Get("ChromaticAberrationPS");
	this->target = target;
	this->strength = 0.0f;
	this->time = 0.0f;

	this->vertex[0].pos = Vector3(0, 0, 0);
	this->vertex[1].pos = Vector3(this->size.x, 0, 0);
	this->vertex[2].pos = Vector3(0, this->size.y, 0);
	this->vertex[3].pos = Vector3(this->size.x, this->size.y, 0);
}

void ChromaticAberration::Draw(void)
{
	auto pDevice = Direct3D::GetDevice();

	// 現在レンダーターゲットの保存
	LPDIRECT3DSURFACE9 pSurface;
	LPDIRECT3DSURFACE9 pDepthSurface;

	// レンダーターゲットの設定
	pDevice->GetRenderTarget(0, &pSurface);
	pDevice->GetDepthStencilSurface(&pDepthSurface);
	pDevice->SetRenderTarget(0, this->target->pSurface);
	pDevice->SetDepthStencilSurface(this->target->pDepthSurface);
	
	// シェーダーの設定
	pDevice->SetPixelShader(this->pshader->pD3DShader);

	this->pshader->SetFloat("time", this->time);
	this->pshader->SetFloat("strength", this->strength);
	this->pshader->SetFloat("scale", GetScale());

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, this->pTexture->pDXTex);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, this->vertex, sizeof(Vertex2D));


	// 元の設定に戻る
	pDevice->SetPixelShader(NULL);
	pDevice->SetRenderTarget(0, pSurface);
	pDevice->SetDepthStencilSurface(pDepthSurface);

	SafeRelease(pSurface);
	SafeRelease(pDepthSurface);
}

float ChromaticAberration::GetScale(void)
{
	return 0.07f * (1.0f + 0.2f*sinf(this->time * 5.0f));
}
