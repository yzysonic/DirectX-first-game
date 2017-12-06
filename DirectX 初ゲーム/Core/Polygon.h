#pragma once

#include "Common.h"
#include "Texture.h"
#include "Drawable.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	RECT_NUM_VERTEX		(4)		// ���_��
#define	RECT_NUM_POLYGON	(2)		// �|���S����

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3D�|���S�����_�t�H�[�}�b�g( ���_���W / �@�� / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
class ObjectBase;

struct Vertex2D
{
	Vector3 pos;		// ���_���W
	float rhw;			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	Color diffuse;		// ���ˌ�
	Vector2 uv;			// �e�N�X�`�����W
};

struct Vertex3D
{
	Vector3 pos;
	Vector3 nor;
	Color diffuse;
	Vector2 uv;
};

class RectPolygon2D : public Drawable
{
public:
	Vertex2D vertex[RECT_NUM_VERTEX];		// ���_���i�[���[�N
	Texture *pTexture;						// �e�N�X�`���[�ւ̃|�C���^
	float radius;							// ���_�v�Z�p���a
	float baseAngle;						// ���_�v�Z�p�p�x
	
	RectPolygon2D(ObjectBase* object, Layer layer, Texture* texture, RendererType rendType);
	void draw(void) override;
	void setColor(Color color) override;
	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) override;
	Vector2 getSize(void);
	void setSize(float x, float y);
	void setPattern(unsigned int pattern);
protected:
	Vector2 size;	// �\������T�C�Y
	int pattern;	// �\������p�^�[��

private:
	void transformVertex(void);
};

class RectPolygon : public Drawable
{
public:
	Vertex3D vertex[RECT_NUM_VERTEX];
	Texture *pTexture;

	RectPolygon(ObjectBase* object, Layer layer, Texture* texture, RendererType rendType);
	void draw(void) override;
	void setColor(Color color) override;
	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) override;

protected:
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	Vector2 size;

private:
	void setVtxBuff(void);

};
