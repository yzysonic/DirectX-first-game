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

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct _Object Object;

typedef struct _VERTEX_2D
{
	Vector3 vtx;		// ���_���W
	float rhw;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	Color diffuse;		// ���ˌ�
	Vector2 uv;			// �e�N�X�`�����W
} Vertex2D;

typedef struct _RectPolygon
{
	Object *object;							// ���L����I�u�W�F�N�g�ւ̎Q��
	RendererType rendType;					// �`����@�w��
	Vertex2D vertex[RECT_NUM_VERTEX];		// ���_���i�[���[�N
	Texture *pTexture;						// �e�N�X�`���[�ւ̃|�C���^
	Vector2 size;							// �\������T�C�Y
	int pattern;							// �\������p�^�[��
	Layer layer;							// �`��̃��C���[
	int poolIndex;							// ���ʔԍ�
	float radius;							// ���_�v�Z�p���a
	float baseAngle;						// ���_�v�Z�p�p�x
	Color color;							// �F�i�Ǎ���p�j
}RectPolygon;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
RectPolygon* newPolygon(Object* object, Layer layer = LAYER_DEFAULT, TextureName texName = TEX_NONE, RendererType rendType = REND_DEFAULT);
void deletePolygon(RectPolygon* thiz);
void Polygon_SetSize(RectPolygon* thiz, float x, float y);
void Polygon_SetColor(RectPolygon *thiz, D3DCOLOR color);
void Polygon_SetOpacity(RectPolygon *thiz, float opacity);
float Polygon_GetOpacity(RectPolygon *thiz);

#endif
