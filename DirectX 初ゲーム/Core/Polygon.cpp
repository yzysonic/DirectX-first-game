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

	// rhw�̐ݒ�
	this->vertex[0].rhw =
	this->vertex[1].rhw =
	this->vertex[2].rhw =
	this->vertex[3].rhw = 1.0f;

	setTexture(texture);

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

		// �J�����ϊ�
		this->vertex[0].pos -= camera->getTransform()->position;
		this->vertex[1].pos -= camera->getTransform()->position;
		this->vertex[2].pos -= camera->getTransform()->position;
		this->vertex[3].pos -= camera->getTransform()->position;

		// ���e�ϊ�
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

	// �X�N���[���ϊ�
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


	// ���_���W�̐ݒ�(���[�J�����W)
	vertex[0].pos = Vector3(-0.5f*this->size.x, 0.5f*this->size.y, 0.0f);
	vertex[1].pos = Vector3(0.5f*this->size.x, 0.5f*this->size.y, 0.0f);
	vertex[2].pos = Vector3(-0.5f*this->size.x, -0.5f*this->size.y, 0.0f);
	vertex[3].pos = Vector3(0.5f*this->size.x, -0.5f*this->size.y, 0.0f);

	// �@���x�N�g���̐ݒ�
	vertex[0].nor =
	vertex[1].nor =
	vertex[2].nor =
	vertex[3].nor = Vector3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	vertex[0].diffuse = 
	vertex[1].diffuse = 
	vertex[2].diffuse = 
	vertex[3].diffuse = this->color;

	// �e�N�X�`�����W�̐ݒ�
	vertex[0].uv = Vector2(0.0f, 0.0f);
	vertex[1].uv = Vector2(1.0f, 0.0f);
	vertex[2].uv = Vector2(0.0f, 1.0f);
	vertex[3].uv = Vector2(1.0f, 1.0f);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	Direct3D::GetDevice()->CreateVertexBuffer(sizeof(Vertex3D) * RECT_NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);

	setVtxBuff();

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

	setVtxBuff();

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

void RectPolygon::setVtxBuff(void)
{
	Vertex3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_�����o�b�t�@�֓]��
	memcpy(pVtx, &vertex, sizeof(vertex));
	// ���_�f�[�^���A�����b�N����
	this->pVtxBuff->Unlock();

}
