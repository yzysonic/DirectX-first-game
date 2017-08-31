#ifndef _POLYGON_H_
#define _POLYGON_H_

#include"main.h"
#include"Texture.h"
#include"Layer.h"

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
	Object *object;
	Vertex2D vertex[RECT_NUM_VERTEX];		// ���_���i�[���[�N
	Texture *pTexture;						// �e�N�X�`���[�ւ̃|�C���^
	Vector2 size;							// �\������T�C�Y
	int pattern;							// �\������p�^�[��
	Layer layer;							// �`��̃��C���[
	int poolIndex;							// ���ʔԍ�
}RectPolygon;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
RectPolygon* newPolygon(Object* object, Layer layer, TextureName texName = TEX_NONE);
void deletePolygon(RectPolygon* thiz);
void Polygon_UpdateVertex(RectPolygon *thiz);


#endif
