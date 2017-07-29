//=============================================================================
//
// �t�@�C�����Fgraphics.h
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "d3dx9.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")

#define	NUM_VERTEX	(4)		// ���_��
#define	NUM_POLYGON	(2)		// �|���S����

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// ��L���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W(*33)
} VERTEX_2D;

HRESULT InitGraphics(HWND hWnd, int screenWidth, int screenHeight, bool bWindowMode);
void UninitGraphics();
void DrawFrame();



#endif // _GRAPHICS_H_


