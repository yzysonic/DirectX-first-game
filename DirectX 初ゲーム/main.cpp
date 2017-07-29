//=============================================================================
//
// �t�@�C�����Fmain.cpp
// ��ҁFGP11A143 38 �k�q�B
//
//=============================================================================
#include "main.h"
#include "graphics.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define WINDOW_CLASS_NAME		_T("MainClass")			// �E�C���h�E�̃N���X��
#define WINDOW_TITLE			_T("DirectX ���Q�[��")	// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitWindows();
void UninitWindows();
HRESULT InitSystem();
void UninitSystem();
void UpdateFrame();


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;

#ifdef _DEBUG
LPD3DXFONT	g_pD3DXFont = NULL;	// �t�H���g�ւ̃|�C���^
int			g_nCountFPS;		// FPS�J�E���^
#endif


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
	DWORD dwExecLastTime = timeGetTime();
	DWORD dwFPSLastTime = timeGetTime();
	DWORD dwCurrentTime = 0;
	DWORD dwFrameCount = 0;
	timeBeginPeriod(1); // ����\��ݒ�

	//�V�X�e��������
	HRESULT hr;
	hr = InitSystem();

	if (FAILED(hr))
		return (-1);

	// ���b�Z�[�W���[�v
	MSG msg;
	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}


		dwCurrentTime = timeGetTime();

		if ((dwCurrentTime - dwFPSLastTime) >= 500) // 0.5�b���ƂɎ��s
		{
#ifdef _DEBUG
			g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime); // FPS���v��
#endif
			dwFPSLastTime = dwCurrentTime;	//�@FPS�v��������ۑ�
			dwFrameCount = 0;				// �J�E���g���N���A
		}

		if ((dwCurrentTime - dwExecLastTime) >= (1000/60)) // 1/60�b���ƂɎ��s
		{
			dwExecLastTime = dwCurrentTime; // ��������������ۑ�

			UpdateFrame();	// �X�V����
			DrawFrame();	// �`�揈��

			dwFrameCount++; //�����񐔂̃J�E���g�����Z
		}

	}
	
	// �V�X�e���I��
	UninitSystem();

	return (int)msg.wParam;
}


//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

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

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_CLASSDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = g_hInstance;
	wcex.lpszClassName = WINDOW_CLASS_NAME;
	//wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClassEx(&wcex);


	// �E�B���h�E�I�u�W�F�N�g���쐬����.
	g_hWnd = CreateWindowEx(
		0,							//�g���X�^�C���i�C�Ӂj
		WINDOW_CLASS_NAME,			//�N���X��
		WINDOW_TITLE,				//�E�B���h�E���i�^�C�g���j
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,			//�X�^�C��
		(GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2,		//�������̈ʒu
		(GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2,	//�c�����̈ʒu
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
	DestroyWindow(g_hWnd);
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
	HRESULT hr;

	// �E�B���h�E�Y�̏�����
	hr = InitWindows();
	
	// �O���t�B�b�N�X�̏�����
	if (SUCCEEDED(hr))
		hr = InitGraphics(g_hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, true);

	return hr;
}

//=============================================================================
// �V�X�e���̏I������
//=============================================================================
void UninitSystem()
{
	UninitGraphics();
	UninitWindows();
}

//=============================================================================
// �X�V��������
//=============================================================================
void UpdateFrame()
{
	DrawFrame();
}