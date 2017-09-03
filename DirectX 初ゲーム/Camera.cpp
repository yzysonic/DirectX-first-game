#include "Camera.h"

const float speed = CAMERA_SPEED;

void initCamera(Object * thiz)
{
	SetThis(Camera);
	thisCamera->target = NULL;
	thiz->transform->position.z = 5.0f;
}

void updateCamera(Object * thiz)
{
	SetThis(Camera);
	thiz->transform->position = LerpV3(thiz->transform->position, thisCamera->target->position + Vector3(0,0,-1.0f), speed*GetDeltaTime());
}

void uninitCamera(Object * thiz)
{
	SetThis(Camera);
}