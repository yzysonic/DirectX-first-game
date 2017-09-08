#include "FadeScreen.h"

enum AnimeState
{
	Set,
	Run,
	Stop,
};

void UpdateFadeScreen(Object* thiz);

void(*update_fade)(void);
Object *fadeLayer;
float fadeTime;
float timer;
float targetOpacity;
float oldOpacity;
Color fadeColor;
AnimeState state;


void InitFadeScreen()
{
	fadeLayer = newObject(&fadeLayer);
	fadeLayer->polygon = newPolygon(fadeLayer, LAYER_TOP, TEX_NONE, REND_UI);
	fadeLayer->update = &UpdateFadeScreen;

	Polygon_SetSize(fadeLayer->polygon, SCREEN_WIDTH, SCREEN_HEIGHT);
	Polygon_SetOpacity(fadeLayer->polygon, 0.0f);

	state = Stop;

	Object_SetActive(fadeLayer, false);
}

void UpdateFadeScreen(Object* thiz)
{

	switch (state)
	{
	case Set:
		Polygon_SetOpacity(fadeLayer->polygon, targetOpacity);
		state = Stop;
		break;
	case Run:
		if (timer < fadeTime + 0.1f)
		{
			float t = Polygon_GetOpacity(fadeLayer->polygon);
			Polygon_SetOpacity(fadeLayer->polygon, Lerpf(oldOpacity, targetOpacity, timer / fadeTime));
		}
		else
		{
			state = Stop;
		}
		timer += GetDeltaTime();
		break;
	case Stop:
		Object_SetActive(fadeLayer, false);
		break;
	}

}

void UninitFadeScreen()
{
	deleteObject(fadeLayer);
}

void FadeScreen(FadeType type, Color color, float interval)
{
	switch (type)
	{
	case FADE_IN:
		targetOpacity = 0.0f;
		Polygon_SetColor(fadeLayer->polygon, color);
		break;
	case FADE_OUT:
		targetOpacity = 1.0f;
		Polygon_SetColor(fadeLayer->polygon, color);
		break;
	case FADE_IN_BK:
		targetOpacity = 0.0f;
		Polygon_SetColor(fadeLayer->polygon, ColorRGBA(0, 0, 0, 255));
		break;
	case FADE_OUT_BK:
		targetOpacity = 1.0f;
		Polygon_SetColor(fadeLayer->polygon, ColorRGBA(0, 0, 0, 0));
		break;
	case FADE_IN_WH:
		targetOpacity = 0.0f;
		Polygon_SetColor(fadeLayer->polygon, ColorRGBA(255, 255, 255, 255));
		break;
	case FADE_OUT_WH:
		targetOpacity = 1.0f;
		Polygon_SetColor(fadeLayer->polygon, ColorRGBA(255, 255, 255, 0));
		break;
	}

	fadeTime = interval;

	if (fadeTime > 0)
		state = Run;
	else
		state = Set;

	timer = 0;
	oldOpacity = Polygon_GetOpacity(fadeLayer->polygon);
	Object_SetActive(fadeLayer, true);
}

bool FadeFinished(void)
{
	if (state == Stop)
		return true;
	else
		return false;
}
