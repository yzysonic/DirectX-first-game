#pragma once

#include "Core.h"

typedef struct _NumberUI
{
	Object *base;
	Object **digitList;
	int digitNum;
	int value;
}NumberUI;

NumberUI* newNumberUI(int digits, int x, int y, TextureName texture_digit, TextureName texture_title = TEX_NONE);
void updateNumberUI(Object *thiz);
void uninitNumberUI(Object *thiz);
void NumberUI_SetNumber(NumberUI *thiz, unsigned int value);
void NumberUI_SetOffset(NumberUI *thiz, int x, int y);