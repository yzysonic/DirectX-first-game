#include "Renderer.h"
#include "Direct3D.h"
#include "Time.h"
#include "Lerp.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define zmax 100.0f
#define zmin 0.1f


//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
extern LPDIRECT3DDEVICE9 g_pD3DDevice;
char g_DebugText[20][256] = {};

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void DrawDebug();


//=============================================================================
// ������
//=============================================================================
void Renderer::Create(void)
{
	Singleton::Create();

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O���s��Ȃ�
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W���Z��������
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// �Q�Ԗڂ̃A���t�@����

	// �����J����
	m_pInstance->camera = &m_pInstance->fixedCamera;


	for (int i = 0; i < (int)Layer::MAX; i++)
	{
		m_pInstance->list[i].reserve(g_PoolSize[i]);
	}

}

//=============================================================================
// �I������
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
// �`�揈��
//=============================================================================
void Renderer::drawFrame()
{
	std::vector<RectPolygon*>	list;
	RectPolygon*			poly;

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), this->camera->backColor, 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		for (int i = 0; i < (int)Layer::MAX; i++)
		{
			list = this->list[i];

			for (size_t j = 0; j < list.size(); j++)
			{
				
				poly = list[j];

				// ���_���W�̍X�V
				transformVertex(poly);

				// ���_�t�H�[�}�b�g�̐ݒ�
				g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

				// �e�N�X�`���̐ݒ�
				g_pD3DDevice->SetTexture(0, poly->pTexture->pDXTex);

				// �|���S���̕`��
				g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, poly->vertex, sizeof(Vertex2D));
			}
		}

		// �f�o�b�O
		DrawDebug();

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	static HRESULT hr;
	hr = g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	// �f�o�C�X���X�g�̌��m
	if (hr == D3DERR_DEVICELOST) {

		// ���A�\�̏ꍇ
		if (g_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			ResetDevice(GetWindowMode());
		}
	}
}

//=============================================================================
// �`�惊�X�g�ɒǉ�
//=============================================================================
void Renderer::addList(RectPolygon* poly)
{
	poly->listIndex = this->list[(int)poly->layer].size();
	this->list[(int)poly->layer].push_back(poly);
}

//=============================================================================
// �`�惊�X�g����폜
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
// �J�����ݒu
//=============================================================================
void Renderer::setCamera(Camera * camera)
{
	if (camera != nullptr)
		this->camera = camera;
	else
		this->camera = &this->fixedCamera;
}


//=============================================================================
// ���_�̍��W�ϊ�
//=============================================================================
void Renderer::transformVertex(RectPolygon *poly)
{
	// ���[���h�ϊ�
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

		// �J�����ϊ�
		poly->vertex[0].vtx -= this->camera->getTransform()->position;
		poly->vertex[1].vtx -= this->camera->getTransform()->position;
		poly->vertex[2].vtx -= this->camera->getTransform()->position;
		poly->vertex[3].vtx -= this->camera->getTransform()->position;

		// ���e�ϊ�
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

	// �X�N���[���ϊ�
	poly->vertex[0].vtx += Vector3(SCREEN_CENTER_X + 0.5f, SCREEN_CENTER_Y + 0.5f, 0.0f);
	poly->vertex[1].vtx += Vector3(SCREEN_CENTER_X + 0.5f, SCREEN_CENTER_Y + 0.5f, 0.0f);
	poly->vertex[2].vtx += Vector3(SCREEN_CENTER_X + 0.5f, SCREEN_CENTER_Y + 0.5f, 0.0f);
	poly->vertex[3].vtx += Vector3(SCREEN_CENTER_X + 0.5f, SCREEN_CENTER_Y + 0.5f, 0.0f);
}

//=============================================================================
// �f�o�b�O������̎擾
//=============================================================================
char *GetDebugText(int line)
{
	return g_DebugText[line];
}

//=============================================================================
// �f�o�b�O�����̕`��
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
