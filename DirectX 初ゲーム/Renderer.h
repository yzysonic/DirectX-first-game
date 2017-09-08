#pragma once

#include "main.h"
#include "Polygon.h"
#include "Object.h"
#include "Layer.h"

// レイヤーごとのプールサイズ
const int g_PoolSize[LAYER_MAX] =
{
	10,			// BG00
	10,			// BG01
	10,			// BG02
	ObjectMax,	// DEFAULT
	10,			// PLAYER
	 1,			// MASK
	30,			// UI00
	10,			// UI01
	10,			// UI02
	10,			// TOP
};

void InitRenderer(void);
void UninitRenderer(void);
void DrawFrame(void);
RectPolygon* Renderer_GetPolygon(Layer layer);
void Renderer_ReleasePolygon(RectPolygon* thiz);
void Renderer_SetBackColor(Color value);
void Renderer_SetCamera(Transform *camera);
char *GetDebugText(int line);
float Renderer_GetFov();
void Renderer_SetFov(float value);