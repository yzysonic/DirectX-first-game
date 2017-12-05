#include "Polygon.h"
#include "Object.h"
#include "Renderer.h"
#include "Lerp.h"


RectPolygon2D::RectPolygon2D(Object* object, Layer layer, TextureName texName, RendererType rendType)
{

	this->object	= object;
	this->layer		= layer;
	this->rendType	= rendType;
	this->pTexture	= GetTexture(texName);
	if(this->pTexture->pDXTex)
		this->size	= this->pTexture->size;
	else
		this->size	= Vector2(100.f, 100.f);

	this->radius = this->size.length()/2;
	this->baseAngle = atan2f(this->size.y, this->size.x);
	this->color.setRGBA(255, 255, 255, 255);

	// rhw�̐ݒ�
	this->vertex[0].rhw =
	this->vertex[1].rhw =
	this->vertex[2].rhw =
	this->vertex[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	this->vertex[0].diffuse =
	this->vertex[1].diffuse =
	this->vertex[2].diffuse =
	this->vertex[3].diffuse = this->color;

	// �e�N�X�`�����W�̐ݒ�
	this->vertex[0].uv = Vector2(0.0f,								0.0f);
	this->vertex[1].uv = Vector2(1.0f / this->pTexture->divideX,	0.0f);
	this->vertex[2].uv = Vector2(0.0f,								1.0f / this->pTexture->divideY);
	this->vertex[3].uv = Vector2(1.0f / this->pTexture->divideX,	1.0f / this->pTexture->divideY);

	this->pattern = 0;

	Renderer::GetInstance()->addList(this);
}

void RectPolygon2D::draw(void)
{
	// ���_���W�̍X�V
	transformVertex();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Direct3D::GetDevice()->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Direct3D::GetDevice()->SetTexture(0, this->pTexture->pDXTex);

	// �|���S���̕`��
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

//=============================================================================
// ���_�̍��W�ϊ�
//=============================================================================
void RectPolygon2D::transformVertex(void)
{
	// ���[���h�ϊ�
	Vector3 pos = this->object->getTransform()->position;
	Vector3 rot = this->object->getTransform()->getRotation();
	Vector3 radius = this->radius * this->object->getTransform()->scale;

	this->vertex[0].vtx.x = pos.x - cosf(this->baseAngle - rot.z) * radius.x;
	this->vertex[0].vtx.y = pos.y + sinf(this->baseAngle - rot.z) * radius.y;
	this->vertex[0].vtx.z = pos.z;

	this->vertex[1].vtx.x = pos.x + cosf(this->baseAngle + rot.z) * radius.x;
	this->vertex[1].vtx.y = pos.y + sinf(this->baseAngle + rot.z) * radius.y;
	this->vertex[1].vtx.z = pos.z;

	this->vertex[2].vtx.x = pos.x - cosf(this->baseAngle + rot.z) * radius.x;
	this->vertex[2].vtx.y = pos.y - sinf(this->baseAngle + rot.z) * radius.y;
	this->vertex[2].vtx.z = pos.z;

	this->vertex[3].vtx.x = pos.x + cosf(this->baseAngle - rot.z) * radius.x;
	this->vertex[3].vtx.y = pos.y - sinf(this->baseAngle - rot.z) * radius.y;
	this->vertex[3].vtx.z = pos.z;

	if (this->rendType == RendererType::Classic2D)
	{
		Camera *camera = Renderer::GetInstance()->getCamera();

		// �J�����ϊ�
		this->vertex[0].vtx -= camera->getTransform()->position;
		this->vertex[1].vtx -= camera->getTransform()->position;
		this->vertex[2].vtx -= camera->getTransform()->position;
		this->vertex[3].vtx -= camera->getTransform()->position;

		// ���e�ϊ�
		float fov = Lerpf(this->vertex[0].vtx.z, 1.0f, camera->fov);
		this->vertex[0].vtx.x /= this->vertex[0].vtx.z / fov;
		this->vertex[0].vtx.y /= this->vertex[0].vtx.z / fov;
		this->vertex[0].vtx.z = (this->vertex[0].vtx.z - camera->view_near_z) / (camera->view_far_z - camera->view_near_z);

		this->vertex[1].vtx.x /= this->vertex[1].vtx.z / fov;
		this->vertex[1].vtx.y /= this->vertex[1].vtx.z / fov;
		this->vertex[1].vtx.z = (this->vertex[1].vtx.z - camera->view_near_z) / (camera->view_far_z - camera->view_near_z);

		this->vertex[2].vtx.x /= this->vertex[2].vtx.z / fov;
		this->vertex[2].vtx.y /= this->vertex[2].vtx.z / fov;
		this->vertex[2].vtx.z = (this->vertex[2].vtx.z - camera->view_near_z) / (camera->view_far_z - camera->view_near_z);

		this->vertex[3].vtx.x /= this->vertex[3].vtx.z / fov;
		this->vertex[3].vtx.y /= this->vertex[3].vtx.z / fov;
		this->vertex[3].vtx.z = (this->vertex[3].vtx.z - camera->view_near_z) / (camera->view_far_z - camera->view_near_z);

	}

	// �X�N���[���ϊ�
	this->vertex[0].vtx.y = -this->vertex[0].vtx.y;
	this->vertex[1].vtx.y = -this->vertex[1].vtx.y;
	this->vertex[2].vtx.y = -this->vertex[2].vtx.y;
	this->vertex[3].vtx.y = -this->vertex[3].vtx.y;

	this->vertex[0].vtx += Vector3(SystemParameters::ResolutionX / 2.f, SystemParameters::ResolutionY / 2.f, 0.0f);
	this->vertex[1].vtx += Vector3(SystemParameters::ResolutionX / 2.f, SystemParameters::ResolutionY / 2.f, 0.0f);
	this->vertex[2].vtx += Vector3(SystemParameters::ResolutionX / 2.f, SystemParameters::ResolutionY / 2.f, 0.0f);
	this->vertex[3].vtx += Vector3(SystemParameters::ResolutionX / 2.f, SystemParameters::ResolutionY / 2.f, 0.0f);

}

RectPolygon::RectPolygon(Object * object, Layer layer, TextureName texName, RendererType rendType)
{

}

void RectPolygon::draw(void)
{
	D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate, mtxView;
	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();

	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, this->object->getTransform()->scale.x, this->object->getTransform()->scale.y, this->object->getTransform()->scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, this->object->getTransform()->getRotation().y, this->object->getTransform()->getRotation().x, this->object->getTransform()->getRotation().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		
	// ���s�ړ��𔽉f(�n�`��z�u���Ă���)
	D3DXMatrixTranslation(&mtxTranslate, this->object->getTransform()->position.x, this->object->getTransform()->position.y, this->object->getTransform()->position.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	// �r���[�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &Renderer::GetInstance()->getCamera()->getViewMatrix(false));
	// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &Renderer::GetInstance()->getCamera()->getProjectionMatrix(false));

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, this->pVtxBuff, 0, sizeof(Vertex3D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this->pTexture->pDXTex);
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, RECT_NUM_POLYGON);

}

void RectPolygon::setColor(Color color)
{

}

void RectPolygon::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{

}