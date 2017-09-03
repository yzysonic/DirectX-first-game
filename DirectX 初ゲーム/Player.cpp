#include "Player.h"
#include "Polygon.h"
#include "Input.h"
#include "Direct3D.h"
#include "Time.h"


void initPlayer(Object *thiz)
{
	thiz->polygon = newPolygon(thiz, LAYER_PLAYER, TEX_PLAYER);
	thiz->transform->position = Vector3(0.0f, 0.0f, 0.0f);
	
}

void updatePlayer(Object *thiz)
{
	float speed = 5.0f;
	float boost = 1.0f;
	
	if (GetKeyboardPress(DIK_LSHIFT))
		boost = 2.0f;

	if (GetKeyboardPress(DIK_LCONTROL))
	{
		if (GetKeyboardPress(DIK_DOWN))
		{
			thiz->transform->position.z -= 0.02f*boost;
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			thiz->transform->position.z += 0.02f*boost;
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_DOWN))
		{
			thiz->transform->position.y += speed*boost;
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			thiz->transform->position.y -= speed*boost;
		}
	}

	if (GetKeyboardPress(DIK_RIGHT))
	{
		thiz->transform->position.x += speed*boost;
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		thiz->transform->position.x -= speed*boost;
	}

}

void uninitPlayer(Object *thiz)
{
}
