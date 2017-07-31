#ifndef _SOUND_H_
#define _SOUND_H_

#include <dsound.h>
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum
{	// サウンド通しナンバー(sound.cppの順番と合わせる事)
	BGM_00,
	SE_00,
	SOUND_MAX
};

enum
{	// 再生用フラグ
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT					InitSound( HWND hWnd );	// 初期化
void					UninitSound();			// 後片付け
LPDIRECTSOUNDBUFFER8	LoadSound( int no );	// サウンドのロード
void					PlaySound( LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0 );	// 音ごとに再生
bool					IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );					// 再生中かどうか

#endif