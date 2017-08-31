#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"
#include "Texture.h"
#include "Layer.h"
#include "RendererType.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	RECT_NUM_VERTEX		(4)		// ���_��
#define	RECT_NUM_POLYGON	(2)		// �|���S����

// ���_�t�H�[�}�b�g
#define	FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_2D_NOTEX	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct _Object Object;

typedef struct _Vertex2DLite
{
	Vector3 vtx;		// ���_���W
	float rhw;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	Color diffuse;		// ���ˌ�
} Vertex2DLite;

typedef struct _Vertex2D
{
	Vector3 vtx;		// ���_���W
	float rhw;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	Color diffuse;		// ���ˌ�
	Vector2 uv;			// �e�N�X�`�����W
} Vertex2D;


typedef struct _RectPolygon
{
	Object *object;
	Vertex2D vertex[RECT_NUM_VERTEX];		// ���_���i�[���[�N
	Texture *pTexture;						// �e�N�X�`���[�ւ̃|�C���^
	Vector2 size;							// �\������T�C�Y
	int pattern;							// �\������p�^�[��
	Layer layer;							// �`��̃��C���[
	int poolIndex;							// ���ʔԍ�
	//void(*draw)(RectPolygon* thiz);			// �`�揈��
	RendererType rendererType;				// �`��p�����_���[�^�C�v
}RectPolygon;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
RectPolygon* newPolygon(Object* object, Layer layer = LAYER_DEFAULT, TextureName texName = TEX_NONE, RendererType rendType = REND_POLY);
void deletePolygon(RectPolygon* thiz);
void Polygon_UpdateVertex(RectPolygon *thiz);
void Polygon_SetTexture(RectPolygon *thiz, TextureName texName);
void Polygon_SetColor(RectPolygon *thiz, D3DCOLOR color);


#endif
