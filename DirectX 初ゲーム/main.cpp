//=============================================================================
//
// �t�@�C�����Fmain.cpp
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#include "CoreBase.h"
#include "SceneTitle.h"
#include "GlobalUpdate.h"


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSystem();
void UninitSystem();


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// ������
	if (FAILED(InitSystem()))
		return 1;
	InitGame(new SceneTitle);
	SetGlobalUpdate(&GlobalUpdate);

	// �Q�[�����[�v
	RunGame();

	// �I��
	UninitGame();
	UninitSystem();

	return 0;
}


//=============================================================================
// �V�X�e���̏���������
//=============================================================================
HRESULT InitSystem()
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // ���������[�N�������o
#endif

	HRESULT hr;

	// �E�B���h�E�̏�����
	hr = Window::Init();
	
	// �O���t�B�b�N�X�̏�����
	if (SUCCEEDED(hr))
		hr = Direct3D::Init(Window::GetHWnd(), SCREEN_WIDTH, SCREEN_HEIGHT, Window::GetWindowMode());

	// �C���v�b�g�̏�����
	if (SUCCEEDED(hr))
		hr = InitInput(Window::GetHInstance(), Window::GetHWnd());

	// �T�E���h�̏�����
	if (SUCCEEDED(hr))
		hr = InitSound(Window::GetHWnd());

	return hr;
}

//=============================================================================
// �V�X�e���̏I������
//=============================================================================
void UninitSystem()
{
	UninitSound();
	UninitInput();
	Direct3D::Uninit();
	Window::Uninit();
}