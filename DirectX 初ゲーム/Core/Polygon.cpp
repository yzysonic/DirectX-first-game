#include "Polygon.h"
#include "Object.h"
#include "Renderer.h"
#include "Lerp.h"


RectPolygon2D::RectPolygon2D(ObjectBase* object, Layer layer, Texture *texture, RendererType rendType, std::string render_space) : Drawable(layer, render_space)
{

	this->object	= object;
	this->layer		= layer;
	this->rendType	= rendType;
	this->pattern = 0;

	// rhwの設定
	this->vertex[0].rhw =
	this->vertex[1].rhw =
	this->vertex[2].rhw =
	this->vertex[3].rhw = 1.0f;

	setTexture(texture);

}

void RectPolygon2D::draw(void)
{
	// 頂点座標の更新
	transformVertex();

	// 頂点フォーマットの設定
	Direct3D::GetDevice()->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Direct3D::GetDevice()->SetTexture(0, this->pTexture->pDXTex);

	// ポリゴンの描画
	Direct3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, this->vertex, sizeof(Vertex2D));

}


Vector2 RectPolygon2D::getSize(void)
{
	return this->size;
}


void RectPolygon2D::setSize(float x, float y)
{
	this->size = Vector2(x, y);
	this->radius = this->size.length()/2;
	this->baseAngle = atan2f(this->size.y, this->size.x);
}


void RectPolygon2D::setColor(Color color)
{
	for (int i = 0; i < RECT_NUM_VERTEX; i++)
		this->vertex[i].diffuse = color;
	this->color = color;
}

void RectPolygon2D::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	for (int i = 0; i < RECT_NUM_VERTEX; i++)
		this->vertex[i].diffuse.setRGBA(r, g, b, a);
	this->color.setRGBA(r, g, b, a);
}

void RectPolygon2D::setPattern(unsigned int pattern)
{
	this->pattern = pattern;

	int x = pattern % this->pTexture->divideX;
	int y = pattern / this->pTexture->divideX;
	Vector2 size = Vector2(1.0f / this->pTexture->divideX, 1.0f / this->pTexture->divideY);

	this->vertex[0].uv = Vector2(x*size.x, y*size.y);
	this->vertex[1].uv = Vector2(x*size.x + size.x, y*size.y);
	this->vertex[2].uv = Vector2(x*size.x, y*size.y + size.y);
	this->vertex[3].uv = Vector2(x*size.x + size.x, y*size.y + size.y);
}

void RectPolygon2D::setTexture(Texture * texture)
{
	this->pTexture	= texture;
	this->size	= this->pTexture->size;

	this->radius = this->size.length()/2;
	this->baseAngle = atan2f(this->size.y, this->size.x);
	if (this->pTexture != Texture::none && this->pTexture->pDXTex == nullptr)
		this->color.setRGBA(255, 0, 255, 255);
	else
		this->color.setRGBA(255, 255, 255, 255);

	// 反射光の設定
	this->vertex[0].diffuse =
	this->vertex[1].diffuse =
	this->vertex[2].diffuse =
	this->vertex[3].diffuse = this->color;

	// テクスチャ座標の設定
	this->vertex[0].uv = Vector2(0.0f,								0.0f);
	this->vertex[1].uv = Vector2(1.0f / this->pTexture->divideX,	0.0f);
	this->vertex[2].uv = Vector2(0.0f,								1.0f / this->pTexture->divideY);
	this->vertex[3].uv = Vector2(1.0f / this->pTexture->divideX,	1.0f / this->pTexture->divideY);

}

//=============================================================================
// 頂点の座標変換
//=============================================================================
void RectPolygon2D::transformVertex(void)
{
	// ワールド変換
	Vector3 pos = this->object->getTransform()->position;
	Vector3 rot = this->object->getTransform()->getRotation();
	Vector3 radius = this->radius * this->object->getTransform()->scale;

	this->vertex[0].pos.x = pos.x - cosf(this->baseAngle - rot.z) * radius.x;
	this->vertex[0].pos.y = pos.y + sinf(this->baseAngle - rot.z) * radius.y;
	this->vertex[0].pos.z = pos.z;

	this->vertex[1].pos.x = pos.x + cosf(this->baseAngle + rot.z) * radius.x;
	this->vertex[1].pos.y = pos.y + sinf(this->baseAngle + rot.z) * radius.y;
	this->vertex[1].pos.z = pos.z;

	this->vertex[2].pos.x = pos.x - cosf(this->baseAngle + rot.z) * radius.x;
	this->vertex[2].pos.y = pos.y - sinf(this->baseAngle + rot.z) * radius.y;
	this->vertex[2].pos.z = pos.z;

	this->vertex[3].pos.x = pos.x + cosf(this->baseAngle - rot.z) * radius.x;
	this->vertex[3].pos.y = pos.y - sinf(this->baseAngle - rot.z) * radius.y;
	this->vertex[3].pos.z = pos.z;

	Camera *camera = Renderer::GetInstance()->getCamera(this->render_space_index);

	if (this->rendType == RendererType::Classic2D)
	{

		// カメラ変換
		this->vertex[0].pos -= camera->getTransform()->position;
		this->vertex[1].pos -= camera->getTransform()->position;
		this->vertex[2].pos -= camera->getTransform()->position;
		this->vertex[3].pos -= camera->getTransform()->position;

		// 投影変換
		float fov = Lerpf(this->vertex[0].pos.z, 1.0f, camera->fov);
		this->vertex[0].pos.x /= this->vertex[0].pos.z / fov;
		this->vertex[0].pos.y /= this->vertex[0].pos.z / fov;
		this->vertex[0].pos.z = (this->vertex[0].pos.z - camera->view_near_z) / (camera->view_far_z - camera->view_near_z);

		this->vertex[1].pos.x /= this->vertex[1].pos.z / fov;
		this->vertex[1].pos.y /= this->vertex[1].pos.z / fov;
		this->vertex[1].pos.z = (this->vertex[1].pos.z - camera->view_near_z) / (camera->view_far_z - camera->view_near_z);

		this->vertex[2].pos.x /= this->vertex[2].pos.z / fov;
		this->vertex[2].pos.y /= this->vertex[2].pos.z / fov;
		this->vertex[2].pos.z = (this->vertex[2].pos.z - camera->view_near_z) / (camera->view_far_z - camera->view_near_z);

		this->vertex[3].pos.x /= this->vertex[3].pos.z / fov;
		this->vertex[3].pos.y /= this->vertex[3].pos.z / fov;
		this->vertex[3].pos.z = (this->vertex[3].pos.z - camera->view_near_z) / (camera->view_far_z - camera->view_near_z);

	}

	// スクリーン変換
	this->vertex[0].pos.y = -this->vertex[0].pos.y;
	this->vertex[1].pos.y = -this->vertex[1].pos.y;
	this->vertex[2].pos.y = -this->vertex[2].pos.y;
	this->vertex[3].pos.y = -this->vertex[3].pos.y;

	this->vertex[0].pos += Vector3(camera->render_target->size.x / 2.f, camera->render_target->size.y / 2.f, 0.0f);
	this->vertex[1].pos += Vector3(camera->render_target->size.x / 2.f, camera->render_target->size.y / 2.f, 0.0f);
	this->vertex[2].pos += Vector3(camera->render_target->size.x / 2.f, camera->render_target->size.y / 2.f, 0.0f);
	this->vertex[3].pos += Vector3(camera->render_target->size.x / 2.f, camera->render_target->size.y / 2.f, 0.0f);

}

RectPolygon::RectPolygon(ObjectBase * object, Layer layer, Texture* texture, RendererType rendType) :  Drawable(layer)
{
	this->object = object;
	this->layer = layer;
	this->rendType = rendType;
	this->pTexture = texture;
	this->size = this->pTexture->size;
	if (this->pTexture != Texture::none && this->pTexture->pDXTex == nullptr)
		this->color.setRGBA(255, 0, 255, 255);
	else
		this->color.setRGBA(255, 255, 255, 255);


	// 頂点座標の設定(ローカル座標)
	vertex[0].pos = Vector3(-0.5f*this->size.x, 0.5f*this->size.y, 0.0f);
	vertex[1].pos = Vector3(0.5f*this->size.x, 0.5f*this->size.y, 0.0f);
	vertex[2].pos = Vector3(-0.5f*this->size.x, -0.5f*this->size.y, 0.0f);
	vertex[3].pos = Vector3(0.5f*this->size.x, -0.5f*this->size.y, 0.0f);

	// 法線ベクトルの設定
	vertex[0].nor =
	vertex[1].nor =
	vertex[2].nor =
	vertex[3].nor = Vector3(0.0f, 0.0f, -1.0f);

	// 反射光の設定
	vertex[0].diffuse = 
	vertex[1].diffuse = 
	vertex[2].diffuse = 
	vertex[3].diffuse = this->color;

	// テクスチャ座標の設定
	vertex[0].uv = Vector2(0.0f, 0.0f);
	vertex[1].uv = Vector2(1.0f, 0.0f);
	vertex[2].uv = Vector2(0.0f, 1.0f);
	vertex[3].uv = Vector2(1.0f, 1.0f);

	// オブジェクトの頂点バッファを生成
	Direct3D::GetDevice()->CreateVertexBuffer(sizeof(Vertex3D) * RECT_NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&this->pVtxBuff,			// 頂点バッファインターフェースへのポインタ
		NULL);

	setVtxBuff();

}

void RectPolygon::draw(void)
{
	D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate, mtxView;
	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();

	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&mtxWorld);
	// スケールを反映
	D3DXMatrixScaling(&mtxScl, this->object->getTransform()->scale.x, this->object->getTransform()->scale.y, this->object->getTransform()->scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, this->object->getTransform()->getRotation().y, this->object->getTransform()->getRotation().x, this->object->getTransform()->getRotation().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		
	// 平行移動を反映(地形を配置している)
	D3DXMatrixTranslation(&mtxTranslate, this->object->getTransform()->position.x, this->object->getTransform()->position.y, this->object->getTransform()->position.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	// ビューマトリクスの設定
	pDevice->SetTransform(D3DTS_VIEW, &Renderer::GetInstance()->getCamera()->getViewMatrix(false));
	// プロジェクションマトリクスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &Renderer::GetInstance()->getCamera()->getProjectionMatrix(false));

	setVtxBuff();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, this->pVtxBuff, 0, sizeof(Vertex3D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	pDevice->SetTexture(0, this->pTexture->pDXTex);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, RECT_NUM_POLYGON);

}

void RectPolygon::setColor(Color color)
{

}

void RectPolygon::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{

}

void RectPolygon::setVtxBuff(void)
{
	Vertex3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報をバッファへ転送
	memcpy(pVtx, &vertex, sizeof(vertex));
	// 頂点データをアンロックする
	this->pVtxBuff->Unlock();

}
