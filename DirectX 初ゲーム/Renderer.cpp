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
// �\���̐錾
//=============================================================================
typedef struct _PolygonPool
{
	RectPolygon *polygon = NULL;
	int activeTop = -1;
}PolygonPool;

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
extern LPDIRECT3DDEVICE9 g_pD3DDevice;
PolygonPool	g_PolygonPool[LAYER_MAX];
Transform g_FixedCamera;
Transform *g_Camera;
int g_PoolSize[LAYER_MAX];
char g_DebugText[10][256] = {};
float g_fov;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void TransformVertex(RectPolygon *thiz);
void DrawDebug();


//=============================================================================
// ������
//=============================================================================
void InitRenderer(void)
{
	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O���s��Ȃ�
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);		// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);		// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W���Z��������
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// �Q�Ԗڂ̃A���t�@����

	// �����J����
	g_Camera = &g_FixedCamera;
	g_fov = 0.0f;

	// �|���S�����g���������̊m��
	if (g_PolygonPool[0].polygon != NULL)
		return;

	g_PoolSize[LAYER_BG_00]		= POOL_SIZE_BG_00;
	g_PoolSize[LAYER_BG_01]		= POOL_SIZE_BG_01;
	g_PoolSize[LAYER_BG_02]		= POOL_SIZE_BG_02;
	g_PoolSize[LAYER_DEFAULT]	= POOL_SIZE_DEFAULT;
	g_PoolSize[LAYER_PLAYER]	= POOL_SIZE_PLAYER;
	g_PoolSize[LAYER_UI_00]		= POOL_SIZE_UI_00;
	g_PoolSize[LAYER_UI_01]		= POOL_SIZE_UI_01;
	g_PoolSize[LAYER_UI_02]		= POOL_SIZE_UI_02;

	for (int i = 0; i < LAYER_MAX; i++)
	{
		g_PolygonPool[i].polygon = (RectPolygon*)malloc(sizeof(RectPolygon)*g_PoolSize[i]);
	}

}

//=============================================================================
// �I������
//=============================================================================
void UninitRenderer(void)
{
	for (int i = 0; i < LAYER_MAX; i++)
	{
		SafeDelete(g_PolygonPool[i].polygon);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFrame()
{
	PolygonPool*		pool;
	RectPolygon*		poly;

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(200, 200, 200, 255), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		for (int i = 0; i < LAYER_MAX; i++)
		{
			pool = &g_PolygonPool[i];

			for (int j = 0; j <= pool->activeTop; j++)
			{
				
				poly = &pool->polygon[j];

				// ���_���W�̍X�V
				TransformVertex(poly);

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
// �|���S�������̎擾
//=============================================================================
RectPolygon * Renderer_GetPolygon(Layer layer)
{
	PolygonPool* pool = &g_PolygonPool[layer];

	if (pool->activeTop < g_PoolSize[layer] - 1)
	{
		RectPolygon* thiz = &pool->polygon[++pool->activeTop];
		thiz->layer = layer;
		thiz->poolIndex = pool->activeTop;
		return thiz;
	}
	else
		return NULL;
}

//=============================================================================
// �|���S�������̎ߕ�
//=============================================================================
void Renderer_ReleasePolygon(RectPolygon * thiz)
{
	PolygonPool* pool = &g_PolygonPool[thiz->layer];
	pool->polygon[thiz->poolIndex] = pool->polygon[pool->activeTop--];
}

//=============================================================================
// �J�����ݒu
//=============================================================================
void Renderer_SetCamera(Transform * camera)
{
	if (camera != NULL)
		g_Camera = camera;
	else
		g_Camera = &g_FixedCamera;
}

//=============================================================================
// ���_�̍��W�ϊ�
//=============================================================================
void TransformVertex(RectPolygon *thiz)
{
	// ���[���h�ϊ�
	Vector3 pos = thiz->object->transform->position;
	Vector3 rot = thiz->object->transform->rotation;
	Vector3 radius = thiz->radius * thiz->object->transform->scale;

	thiz->vertex[0].vtx.x = pos.x - cosf(thiz->baseAngle + rot.z) * radius.x;
	thiz->vertex[0].vtx.y = pos.y - sinf(thiz->baseAngle + rot.z) * radius.y;
	thiz->vertex[0].vtx.z = pos.z;

	thiz->vertex[1].vtx.x = pos.x + cosf(thiz->baseAngle - rot.z) * radius.x;
	thiz->vertex[1].vtx.y = pos.y - sinf(thiz->baseAngle - rot.z) * radius.y;
	thiz->vertex[1].vtx.z = pos.z;

	thiz->vertex[2].vtx.x = pos.x - cosf(thiz->baseAngle - rot.z) * radius.x;
	thiz->vertex[2].vtx.y = pos.y + sinf(thiz->baseAngle - rot.z) * radius.y;
	thiz->vertex[2].vtx.z = pos.z;

	thiz->vertex[3].vtx.x = pos.x + cosf(thiz->baseAngle + rot.z) * radius.x;
	thiz->vertex[3].vtx.y = pos.y + sinf(thiz->baseAngle + rot.z) * radius.y;
	thiz->vertex[3].vtx.z = pos.z;

	// �J�����ϊ�
	thiz->vertex[0].vtx -= g_Camera->position;
	thiz->vertex[1].vtx -= g_Camera->position;
	thiz->vertex[2].vtx -= g_Camera->position;
	thiz->vertex[3].vtx -= g_Camera->position;

	// ���e�ϊ�
	float fov = Lerpf(thiz->vertex[0].vtx.z, 1.0f, g_fov);
	thiz->vertex[0].vtx.x /= thiz->vertex[0].vtx.z / fov;
	thiz->vertex[0].vtx.y /= thiz->vertex[0].vtx.z / fov;
	thiz->vertex[0].vtx.z = (thiz->vertex[0].vtx.z - zmin) / (zmax - zmin);

	thiz->vertex[1].vtx.x /= thiz->vertex[1].vtx.z / fov;
	thiz->vertex[1].vtx.y /= thiz->vertex[1].vtx.z / fov;
	thiz->vertex[1].vtx.z = (thiz->vertex[1].vtx.z - zmin) / (zmax - zmin);

	thiz->vertex[2].vtx.x /= thiz->vertex[2].vtx.z / fov;
	thiz->vertex[2].vtx.y /= thiz->vertex[2].vtx.z / fov;
	thiz->vertex[2].vtx.z = (thiz->vertex[2].vtx.z - zmin) / (zmax - zmin);

	thiz->vertex[3].vtx.x /= thiz->vertex[3].vtx.z / fov;
	thiz->vertex[3].vtx.y /= thiz->vertex[3].vtx.z / fov;
	thiz->vertex[3].vtx.z = (thiz->vertex[3].vtx.z - zmin) / (zmax - zmin);

	// �X�N���[���ϊ�
	thiz->vertex[0].vtx += Vector3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	thiz->vertex[1].vtx += Vector3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	thiz->vertex[2].vtx += Vector3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	thiz->vertex[3].vtx += Vector3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
}

//=============================================================================
// �f�o�b�O������̎擾
//=============================================================================
char *GetDebugText(int line)
{
	return g_DebugText[line];
}

float Renderer_GetFov()
{
	return g_fov;
}

void Renderer_SetFov(float value)
{
	g_fov = value;
}

//=============================================================================
// �f�o�b�O�����̕`��
//=============================================================================
void DrawDebug()
{
	RECT rect;
	for (int i = 0; i < 10; i++)
	{
		rect = { 0,i*20,SCREEN_WIDTH,SCREEN_HEIGHT };
		GetFont()->DrawText(NULL, g_DebugText[i], -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}
