//=============================================================================
//
// ファイル名：input.h
// 作者：GP11A143 38 楊子毅
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include <dinput.h>
#include <windows.h>
#include <tchar.h>
#include "Vector.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************


// プログラム分けするときに使う
#define	USE_KEYBOARD										// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE											// 宣言するとマウスで操作可能になる
#define	USE_PAD												// 宣言するとパッドで操作可能になる


/* game pad情報 */
#define BUTTON_UP		0x00000001l	// 方向キー上
#define BUTTON_DOWN		0x00000002l	// 方向キー下
#define BUTTON_LEFT		0x00000004l	// 方向キー左
#define BUTTON_RIGHT	0x00000008l	// 方向キー右
#define BUTTON_SQ		0x00000010l	// □ボタン(.rgbButtons[0]&0x80)
#define BUTTON_CR		0x00000020l	// ×ボタン(.rgbButtons[1]&0x80)
#define BUTTON_CI		0x00000040l	// ○ボタン(.rgbButtons[2]&0x80)
#define BUTTON_TR		0x00000080l	// △ボタン(.rgbButtons[3]&0x80)
#define BUTTON_L1		0x00000100l	// L1ボタン(.rgbButtons[4]&0x80)
#define BUTTON_R1		0x00000200l	// R1ボタン(.rgbButtons[5]&0x80)
#define BUTTON_L2		0x00000400l	// L2ボタン(.rgbButtons[6]&0x80)
#define BUTTON_R2		0x00000800l	// R2ボタン(.rgbButtons[7]&0x80)
#define BUTTON_SHARE	0x00001000l	// SHAREボタン(.rgbButtons[8]&0x80)
#define BUTTON_OPTIONS	0x00002000l	// OPTIONSボタン(.rgbButtons[9]&0x80)
#define BUTTON_L3		0x00004000l	// L3ボタン(.rgbButtons[10]&0x80)
#define BUTTON_R3		0x00008000l	// R3ボタン(.rgbButtons[11]&0x80)
#define BUTTON_PS		0x00010000l	// PSボタン(.rgbButtons[12]&0x80)
#define BUTTON_PAD		0x00020000l	// PADボタン(.rgbButtons[13]&0x80)
#define GAMEPADMAX		4			// 同時に接続するジョイパッドの最大数をセット

enum ControlType
{
	kKeyboard = 0x01,
	kMouse = 0x02,
	kPad = 0x04
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardAnyKeyTrigger();

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);      // 左クリックした状態
BOOL IsMouseLeftTriggered(void);    // 左クリックした瞬間
BOOL IsMouseRightPressed(void);     // 右クリックした状態
BOOL IsMouseRightTriggered(void);   // 右クリックした瞬間
BOOL IsMouseCenterPressed(void);    // 中クリックした状態
BOOL IsMouseCenterTriggered(void);  // 中クリックした瞬間
long GetMouseMoveX(void);           // マウスがX方向に動いた相対値
long GetMouseMoveY(void);           // マウスがY方向に動いた相対値
long GetMouseMoveZ(void);           // マウスホイールが動いた相対値
Vector3 GetMousePos(void);          // マウスの座標

//---------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);
float GetPadLX(void);
float GetPadLY(void);
float GetPadRX(void);
float GetPadRY(void);


#endif