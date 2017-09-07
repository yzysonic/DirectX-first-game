#pragma once
#include "Core.h"

#define DEFAULT_FADE_TIME (0.3f)

enum FadeType
{
	FADE_IN,
	FADE_IN_BK,
	FADE_IN_WH,
	FADE_OUT,
	FADE_OUT_BK,
	FADE_OUT_WH
};


void InitFadeScreen(void);
void UninitFadeScreen(void);
void FadeScreen(FadeType type, Color color = 0, float interval = DEFAULT_FADE_TIME);
bool FadeFinished(void);