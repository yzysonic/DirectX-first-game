#pragma once
#include "Core.h"

#define CAMERA_SPEED (5.0f);

typedef struct _Camera
{
	Object *base;
	Transform *target;
}Camera;

void initCamera(Object* thiz);
void updateCamera(Object* thiz);
void uninitCamera(Object* thiz);

Camera* newCamera(Transform* target, Vector3 pos = Vector3(0, 0, -1.0f));