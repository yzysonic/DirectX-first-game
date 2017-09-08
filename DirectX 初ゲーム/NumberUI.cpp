#include "NumberUI.h"

NumberUI* newNumberUI(int digits, int x, int y, TextureName texture_digit, TextureName texture_title)
{
	NumberUI *thiz = NewSubObj(NumberUI);

	thiz->base->transform->position = Vector3((float)x, (float)y, 0.0f);

	if (texture_title != TEX_NONE)
	{
		thiz->base->polygon = newPolygon(thiz->base, LAYER_UI_00, texture_title, REND_UI);
		thiz->base->transform->position.x += GetTexture(texture_title)->size.x / 2;
	}

	thiz->digitNum = digits;

	thiz->digitList = (Object**)malloc(sizeof(Object*)*digits);
	for (int i = 0; i < digits; i++)
	{
		Object* digit = thiz->digitList[i];
		
		digit = newObject(&thiz->digitList[i]);
		digit->polygon = newPolygon(digit, LAYER_UI_00, texture_digit, REND_UI);
		digit->transform->position.x = x + (digits - i - 0.5f)*GetTexture(texture_digit)->size.x;
		digit->transform->position.y = (float)y;
	}

	return thiz;
}

void updateNumberUI(Object * thiz)
{
}

void uninitNumberUI(Object * thiz)
{
	SetThis(NumberUI);

	for (int i = 0; i < thizz->digitNum; i++)
		deleteObject(thizz->digitList[i]);
	SafeDelete(thizz->digitList);
}

void NumberUI_SetNumber(NumberUI *thiz, unsigned int value)
{
	thiz->value = value;
	for (int i = 0; i < thiz->digitNum; i++)
	{
		Polygon_SetPattern(thiz->digitList[i]->polygon, value % 10);
		value /= 10;
	}
}

void NumberUI_SetOffset(NumberUI * thiz, int x, int y)
{
	for (int i = 0; i < thiz->digitNum; i++)
	{
		thiz->digitList[i]->transform->position = thiz->base->transform->position + Vector3((float)x, (float)y, 0.0f);
		thiz->digitList[i]->transform->position.x += (thiz->digitNum - i - 0.5f)*thiz->digitList[i]->polygon->size.x - thiz->base->polygon->size.x/2;
	}
}
