#pragma once

#include "Common.h"
#include "Texture.h"
#include "Layer.h"
#include "RendererType.h"
#include "Color.h"

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
class Object;

struct Vertex2D
{
	Vector3 vtx;		// ���_���W
	float rhw;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	Color diffuse;		// ���ˌ�
	Vector2 uv;			// �e�N�X�`�����W
};

class RectPolygon
{
public:
	Object *object;							// ���L����I�u�W�F�N�g�ւ̎Q��
	RendererType rendType;					// �`����@�w��
	Vertex2D vertex[RECT_NUM_VERTEX];		// ���_���i�[���[�N
	Texture *pTexture;						// �e�N�X�`���[�ւ̃|�C���^
	int listIndex;							// ���ʔԍ�
	float radius;							// ���_�v�Z�p���a
	float baseAngle;						// ���_�v�Z�p�p�x
	
	RectPolygon(Object* object, Layer layer, TextureName texName, RendererType rendType);
	~RectPolygon(void);
	Layer getLayer(void);
	Vector2 getSize(void);
	void setSize(float x, float y);
	Color getColor(void);
	void setColor(Color color);
	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	float getOpacity(void);
	void setOpacity(float value);
	void setPattern(unsigned int pattern);

protected:
	Layer layer;							// �`��̃��C���[
	Vector2 size;							// �\������T�C�Y
	Color color;							// �F
	int pattern;							// �\������p�^�[��

	friend class Renderer;
};

