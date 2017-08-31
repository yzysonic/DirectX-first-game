#pragma once

#include "main.h"
#include "Polygon.h"
#include "Layer.h"

void DrawFrame();
RectPolygon* Renderer_GetPolygon(Layer layer);
void Renderer_ReleasePolygon(RectPolygon* thiz);