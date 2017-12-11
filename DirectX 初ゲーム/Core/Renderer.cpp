#include "Renderer.h"
#include "Direct3D.h"
#include "RenderSpace.h"


//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
char g_DebugText[40][256] = {};

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

	LPDIRECT3DDEVICE9 pDevice = Direct3D::GetDevice();

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// �J�����O���s��Ȃ�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);					// Z�o�b�t�@���g�p
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W���Z��������
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// �Q�Ԗڂ̃A���t�@����

	// �����J����
	//m_pInstance->camera = &m_pInstance->fixedCamera;


	//for (int i = 0; i < (int)Layer::MAX; i++)
	//{
	//	m_pInstance->list[i].reserve(g_PoolSize[i]);
	//}

	RenderSpace::Add("default");

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
		//m_pInstance->list[i].clear();
	}

	for (int i = RenderSpace::RenderSpaceCount(); i >=0; i--)
		RenderSpace::Delete(i);

	Singleton::Destroy();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Renderer::DrawFrame()
{
	auto pDevice = Direct3D::GetDevice();

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		RenderSpace::Draw();

		// �f�o�b�O
		DrawDebug();

		// Direct3D�ɂ��`��̏I��
		pDevice->EndScene();
	}

	pDevice->SetRenderTarget(0, RenderTarget::BackBuffer()->pSurface);
	pDevice->SetDepthStencilSurface(RenderTarget::BackBuffer()->pDepthSurface);

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	static HRESULT hr;
	hr = pDevice->Present(NULL, NULL, NULL, NULL);

	// �f�o�C�X���X�g�̌��m
	if (hr == D3DERR_DEVICELOST) {

		// ���A�\�̏ꍇ
		if (pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			Direct3D::ResetDevice();
		}
	}
}

//=============================================================================
// �`�惊�X�g�ɒǉ�
//=============================================================================
//void Renderer::addList(Drawable* poly)
//{
//	poly->list_index = this->list[(int)poly->layer].size();
//	this->list[(int)poly->layer].push_back(poly);
//}

//=============================================================================
// �`�惊�X�g����폜
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
// �J�����ݒu
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
	for (int i = 0; i < 40; i++)
	{
		rect = { 0,i * 20,SystemParameters::ResolutionX,SystemParameters::ResolutionY };
		Direct3D::GetFont()->DrawText(NULL, g_DebugText[i], -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}
