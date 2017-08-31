#include "Renderer.h"
#include "Direct3D.h"
#include "SceneGame.h"
#include "Time.h"

const int g_PoolSize = ObjectMax / LAYER_MAX;

typedef struct _PolygonPool
{
	RectPolygon polygon[g_PoolSize];
	int activeTop = -1;
}PolygonPool;


PolygonPool	g_PolygonPool[LAYER_MAX];



//=============================================================================
// �`�揈��
//=============================================================================
void DrawFrame()
{
	LPDIRECT3DDEVICE9	device = GetDevice();
	PolygonPool*		pool;
	RectPolygon*		poly;

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(200, 200, 200, 255), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(device->BeginScene()))
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		device->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < LAYER_MAX; i++)
		{
			pool = &g_PolygonPool[i];

			for (int j = 0; j <= pool->activeTop; j++)
			{
				
				poly = &pool->polygon[j];

				// ���_���W�̍X�V
				Polygon_UpdateVertex(poly);
				device->SetFVF(FVF_VERTEX_2D);
				// �e�N�X�`���̐ݒ�
				device->SetTexture(0, poly->pTexture->pDXTex);

				// �|���S���̕`��
				device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, poly->vertex, sizeof(Vertex2D));
			}
		}

		// Direct3D�ɂ��`��̏I��
		device->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	device->Present(NULL, NULL, NULL, NULL);
}

RectPolygon * Renderer_GetPolygon(Layer layer)
{
	PolygonPool* pool = &g_PolygonPool[layer];

	if (pool->activeTop < g_PoolSize - 1)
	{
		RectPolygon* thiz = &pool->polygon[++pool->activeTop];
		thiz->layer = layer;
		thiz->poolIndex = pool->activeTop;
		return thiz;
	}
	else
		return NULL;
}

void Renderer_ReleasePolygon(RectPolygon * thiz)
{
	PolygonPool* pool = &g_PolygonPool[thiz->layer];
	pool->polygon[thiz->poolIndex] = pool->polygon[pool->activeTop--];
}
