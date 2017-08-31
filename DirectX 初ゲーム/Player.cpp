#include "Player.h"
#include "Polygon.h"
#include "Input.h"
#include "Direct3D.h"
#include "Time.h"

Player* newPlayer(void)
{
	Player* thiz = New(Player);
	thiz->base = newObject(ObjType_Player);
	return thiz;
}

void deletePlayer(Player * thiz)
{
	deleteObject(thiz->base);
	SafeDelete(thiz);
	thiz = NULL;
}

void initPlayer(Object *thiz)
{
	thiz->polygon = newPolygon(thiz, LAYER_PLAYER, TEX_PLAYER);
	thiz->transform->position = Vector3(600, 600, 0.5f);
	
}

void updatePlayer(Object *thiz)
{
	if (GetKeyboardPress(DIK_DOWN))
	{
		thiz->transform->position.y += 5.0f;
	}
	else if (GetKeyboardPress(DIK_UP))
	{
		thiz->transform->position.y -= 5.0f;
	}

	if (GetKeyboardPress(DIK_RIGHT))
	{
		thiz->transform->position.x += 5.0f;
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		thiz->transform->position.x -= 5.0f;
	}

	thiz->transform->position.x += 100*GetDeltaTime();
	if (thiz->transform->position.x > SCREEN_WIDTH)
		thiz->transform->position.x = 0;

}

void uninitPlayer(Object *thiz)
{
}
