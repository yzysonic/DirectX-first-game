#pragma once

#include "main.h"
#include "Polygon.h"
#include "Object.h"
#include "Layer.h"

void InitRenderer(void);
void DrawFrame(void);
RectPolygon* Renderer_GetPolygon(Layer layer);
void Renderer_ReleasePolygon(RectPolygon* thiz);
void Renderer_SetCamera(Transform *camera);
char *GetDebugText(int line);