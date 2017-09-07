#pragma once

#include "main.h"
#include "Polygon.h"
#include "Object.h"
#include "Layer.h"

#define POOL_SIZE_BG_00		10;
#define POOL_SIZE_BG_01		10;
#define POOL_SIZE_BG_02		10;
#define POOL_SIZE_DEFAULT	ObjectMax;
#define POOL_SIZE_PLAYER	10;
#define POOL_SIZE_UI_00		10;
#define POOL_SIZE_UI_01		10;
#define POOL_SIZE_UI_02		10;
#define POOL_SIZE_TOP		10;

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