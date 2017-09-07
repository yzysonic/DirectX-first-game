#include "Player.h"


void initPlayer(Object *thiz)
{
	SetThis(Player);

	thiz->polygon = newPolygon(thiz, LAYER_PLAYER, TEX_PLAYER);
	thiz->collider = newCollider(thiz);
	thiz->transform->position = Vector3(0.0f, 0.0f, 0.0f);
	thiz->transform->scale = Vector3(0.5f, 0.5f, 0.0f);

	thizz->speed = 700.0f;
	thizz->dir = Vector3(0, -1, 0);
	thizz->timer = 0;

}

void updatePlayer(Object *thiz)
{
	SetThis(Player);


	// ˆÚ“®“ü—Í
	D3DXVECTOR3 controlVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_W))
	{
		controlVector += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_S))
	{
		controlVector += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_A))
	{
		controlVector += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_D))
	{
		controlVector += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}

	if ((controlVector.x != 0.0f) || controlVector.y != 0.0f)
	{
		D3DXVec3Normalize(&controlVector, &controlVector);
		thizz->dir = controlVector;
		thiz->transform->rotation.z = atan2f(controlVector.y, controlVector.x) + PI / 2;
	}

	// ‰Á‘¬“ü—Í
	float boost = 1.0f;
	if (GetKeyboardPress(DIK_LSHIFT))
		boost = 2.0f;

	// ˆÚ“®ˆ—
	thiz->transform->position += thizz->speed * boost * controlVector * GetDeltaTime();


	// ƒ}ƒEƒXÆ€‚ÌŒvŽZ

	//if(GetMousePos().x > 0)
	//	thiz->transform->rotation.z = acosf(D3DXVec3Dot(&Vector3(0,-1,0), &GetMousePos()) / (D3DXVec3Length(&GetMousePos())));
	//else
	//	thiz->transform->rotation.z = PI+acosf(-D3DXVec3Dot(&Vector3(0, -1, 0), &GetMousePos()) / (D3DXVec3Length(&GetMousePos())));

	D3DXVec3Normalize(&thizz->dir, &GetMousePos());
	thiz->transform->rotation.z = atan2f(GetMousePos().y, GetMousePos().x) + PI / 2;

	// ’e”­ŽË
	controlVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_UP))
	{
		controlVector += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		controlVector += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		controlVector += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		controlVector += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}

	if ((controlVector.x != 0.0f) || controlVector.y != 0.0f)
	{
		D3DXVec3Normalize(&controlVector, &controlVector);
		thizz->dir = controlVector;
		thiz->transform->rotation.z = atan2f(controlVector.y, controlVector.x) + PI / 2;
	}

	// ’e”­ŽË
	if (
		GetKeyboardPress(DIK_UP)		|| 
		GetKeyboardPress(DIK_DOWN)		|| 
		GetKeyboardPress(DIK_LEFT)		|| 
		GetKeyboardPress(DIK_RIGHT)		|| 
		GetKeyboardPress(DIK_RSHIFT)	|| 
		IsMouseLeftPressed()
		)
	{
		if(thizz->timer >= 0.13f)
		{
			newBullet(thiz->transform, 2000.0f*thizz->dir + thizz->speed * boost * controlVector);
			thizz->timer = 0;
		}
		thizz->timer += GetDeltaTime();
	}


}

void uninitPlayer(Object *thiz)
{

}

void onCollisionPlayer(Object * thiz, Object * other)
{
	if (other->type == Obj_Test)
	{
		Polygon_SetColor(other->polygon, ColorRGBA(255, 255, 255, 255));
	}
}
