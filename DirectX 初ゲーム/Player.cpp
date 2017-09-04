#include "Player.h"
#include "test.h"


void initPlayer(Object *thiz)
{
	SetThis(Player);

	thiz->polygon = newPolygon(thiz, LAYER_PLAYER, TEX_PLAYER);
	thiz->collider = newCollider(thiz);
	thiz->transform->position = Vector3(0.0f, 0.0f, 0.0f);
	thiz->transform->scale = Vector3(0.5f, 0.5f, 0.0f);

	thisPlayer->speed = 500.0f;
}

void updatePlayer(Object *thiz)
{
	SetThis(Player);
	float boost = 1.0f;
	
	if (GetKeyboardPress(DIK_LSHIFT))
		boost = 2.0f;

	D3DXVECTOR3 controlVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (GetKeyboardPress(DIK_LCONTROL))
	{
		if (GetKeyboardPress(DIK_DOWN))
		{
			thiz->transform->position.z -= 2.f*boost*GetDeltaTime();
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			thiz->transform->position.z += 2.f*boost*GetDeltaTime();
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_UP))
		{
			controlVector += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		}
		if (GetKeyboardPress(DIK_DOWN))
		{
			controlVector += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
	}


	if (GetKeyboardPress(DIK_LEFT))
	{
		controlVector += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		controlVector += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}

	float length = powf(controlVector.x, 2.0f) + powf(controlVector.y, 2.0f);
	if (length != 0.0f)
	{
		controlVector /= sqrt(powf(controlVector.x, 2.0f) + powf(controlVector.y, 2.0f));
		thisPlayer->dir = controlVector;
		thiz->transform->rotation.z = atan2f(controlVector.y, controlVector.x) + PI / 2;
	}

	thiz->transform->position += thisPlayer->speed * boost * controlVector * GetDeltaTime();

}

void uninitPlayer(Object *thiz)
{

}

void onCollisionPlayer(Object * thiz, Object * other)
{
	if (other->type == ObjType_Test)
	{
		Polygon_SetColor(other->polygon, ColorRGBA(255, 255, 255, 255));
	}
}
