#include "Renderer.h"
#include "RendererType.h"
#include "Direct3D.h"
#include "SceneGame.h"
#include "Time.h"

const int g_PoolSize = ObjectMax / LAYER_MAX;

typedef struct _PolygonPool
{
	RectPolygon polygon[g_PoolSize];
	int activeTop = -1;
}PolygonPool;

void DrawPoly(RectPolygon *poly);
void DrawTexPoly(RectPolygon *poly);

extern LPDIRECT3DDEVICE9	g_pD3DDevice;
PolygonPool					g_PolygonPool[LAYER_MAX];


//=============================================================================
// ���C���`�揈��
//=============================================================================
void DrawFrame()
{
	PolygonPool* pool;
	RectPolygon* poly;

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

				switch (poly->rendererType)
				{
				case REND_POLY:
					DrawPoly(poly);
					break;
				case REND_TEX_POLY:
					DrawTexPoly(poly);
					break;
				}
			}
		}

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
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


//=============================================================================
// �����_���[�ʂ̕`�揈��
//=============================================================================
void DrawPoly(RectPolygon * poly)
{
	// ���_�t�H�[�}�b�g�̐ݒ�
	g_pD3DDevice->SetFVF(FVF_VERTEX_2D_NOTEX);

	// ���_���W�̍X�V
	Polygon_UpdateVertex(poly);

	// �|���S���̕`��
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, poly->vertex, sizeof(Vertex2D) - sizeof(Vector2));
}
void DrawTexPoly(RectPolygon * poly)
{
	// ���_�t�H�[�}�b�g�̐ݒ�
	g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// ���_���W�̍X�V
	Polygon_UpdateVertex(poly);

	// �e�N�X�`���̐ݒ�
	g_pD3DDevice->SetTexture(0, poly->pTexture->pDXTex);

	// �|���S���̕`��
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, poly->vertex, sizeof(Vertex2D));

}
