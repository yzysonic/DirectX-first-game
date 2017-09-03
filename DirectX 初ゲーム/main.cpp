//=============================================================================
//
// �t�@�C�����Fmain.cpp
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#include "main.h"
#include "Direct3D.h"
#include "Input.h"
#include "Sound.h"
#include "Game.h"



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitWindows();
void UninitWindows();
HRESULT InitSystem();
void UninitSystem();


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
HINSTANCE	g_hInstance = NULL;
HWND		g_hWnd = NULL;
bool		g_bWindowMode = true;


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// ������
	if (FAILED(InitSystem()))
		return 1;
	InitGame();

	// �Q�[�����[�v
	RunGame();

	// �I��
	UninitGame();
	UninitSystem();

	return 0;
}


//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		StopGame();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:		// [ESC]�L�[�������ꂽ
			StopGame();
			break;
		case VK_F9:			// [F9]�L�[�������ꂽ
			SetWindowMode(!g_bWindowMode);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// Windows�̏���������
//=============================================================================
HRESULT InitWindows()
{
	g_hInstance = GetModuleHandle(NULL);

	// �E�B���h�E�N���X��o�^����
	WNDCLASSEX	wcex = {};

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_CLASSDC;
	wcex.lpfnWndProc	= WindowProc;
	wcex.hInstance		= g_hInstance;
	wcex.lpszClassName	= WINDOW_CLASS_NAME;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClassEx(&wcex);


	// �E�B���h�E�I�u�W�F�N�g���쐬����.
	g_hWnd = CreateWindowEx(
		0,							//�g���X�^�C���i�C�Ӂj
		WINDOW_CLASS_NAME,			//�N���X��
		WINDOW_TITLE,				//�E�B���h�E���i�^�C�g���j
		(g_bWindowMode ? (WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU) : WS_POPUP),		//�X�^�C��
		(g_bWindowMode ? ((GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2) : 0),		//�������̈ʒu
		(g_bWindowMode ? ((GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2) : 0),	//�c�����̈ʒu
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,		//��
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	//����
		NULL,						//�e�E�B���h�E
		NULL,						//���j���[
		g_hInstance,				//�A�v���P�[�V�����C���X�^���X�̃n���h��
		NULL						//�E�B���h�E�쐬�f�[�^
	);

	if (g_hWnd == NULL)
		return E_FAIL;

	//�E�B���h�E���X�N���[���ɕ\������
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	return S_OK;
}

//=============================================================================
// Windows�̏I������
//=============================================================================
void UninitWindows()
{
	// �E�B���h�E�̃f�X�g���C�h
	//DestroyWindow(g_hWnd);
	g_hWnd = NULL;

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(WINDOW_CLASS_NAME, g_hInstance);
	g_hInstance = NULL;
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

	// �E�B���h�E�Y�̏�����
	hr = InitWindows();
	
	// �O���t�B�b�N�X�̏�����
	if (SUCCEEDED(hr))
		hr = InitDirect3D(g_hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, g_bWindowMode);

	// �C���v�b�g�̏�����
	if (SUCCEEDED(hr))
		hr = InitInput(g_hInstance, g_hWnd);

	// �T�E���h�̏�����
	if (SUCCEEDED(hr))
		hr = InitSound(g_hWnd);

	return hr;
}

//=============================================================================
// �V�X�e���̏I������
//=============================================================================
void UninitSystem()
{
	UninitSound();
	UninitInput();
	UninitDirect3D();
	UninitWindows();
}


HWND GetHWnd()
{
	return g_hWnd;
}

bool GetWindowMode(void)
{
	return g_bWindowMode;
}

void SetWindowMode(bool windowMode)
{
	if (!ResetDevice(windowMode))
		return;

	if (windowMode)
	{
		SetWindowLong(g_hWnd, GWL_STYLE, WS_CAPTION | WS_THICKFRAME | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU);
		SetWindowPos(g_hWnd, NULL, 
			(GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2,
			0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowLong(g_hWnd, GWL_STYLE, WS_POPUP | WS_EX_TOPMOST);
	}


	ShowWindow(g_hWnd, SW_SHOW);

	g_bWindowMode = windowMode;
}
