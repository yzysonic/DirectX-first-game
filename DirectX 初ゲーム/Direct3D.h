//=============================================================================
//
// �t�@�C�����Fgraphics.h
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#pragma once

#include <d3dx9.h>

class Direct3D
{
public:
	static HRESULT Init(HWND hWnd, int screenWidth, int screenHeight, bool bWindowMode);
	static void Uninit(void);
	static LPDIRECT3DDEVICE9 GetDevice(void);
	static bool ResetDevice(void);
	static LPD3DXFONT GetFont(void);
	static bool SetWindowMode(bool windowMode);

private:
	static LPDIRECT3D9				s_pD3D;			// Direct3D�I�u�W�F�N�g
	static LPDIRECT3DDEVICE9		s_pDevice;	// Device�I�u�W�F�N�g(�`��ɕK�v)
	static LPD3DXFONT				s_pFont;	// �t�H���g
	static D3DPRESENT_PARAMETERS	s_d3dpp;		// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
};

