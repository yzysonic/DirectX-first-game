#include "Billboard.h"
#include "Renderer.h"

void Billboard::Draw(void)
{
	D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate, mtxView;
	auto pDevice = Direct3D::GetDevice();
	auto camera = Renderer::GetInstance()->getCamera();
	
	mtxView = camera->getViewMatrix(false);
	mtxView._41 =
	mtxView._42 =
	mtxView._43 = 0.0f;

	D3DXMatrixInverse(&mtxView, NULL, &mtxView);


	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&mtxWorld);
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, this->object->transform.scale.x, this->object->transform.scale.y, this->object->transform.scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, this->object->transform.getRotation().y, this->object->transform.getRotation().x, this->object->transform.getRotation().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// カメラの回転の逆操作
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxView);

	// 平行移動を反映(地形を配置している)
	D3DXMatrixTranslation(&mtxTranslate, this->object->transform.position.x, this->object->transform.position.y, this->object->transform.position.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	// ビューマトリクスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera->getViewMatrix(false));
	// プロジェクションマトリクスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->getProjectionMatrix(false));

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, this->pVtxBuff, 0, sizeof(Vertex3D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	pDevice->SetTexture(0, this->pTexture->pDXTex);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, RECT_NUM_POLYGON);

}
