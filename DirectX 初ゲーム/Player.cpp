#include "Player.h"

void update_player_muteki(Player *thiz);

void initPlayer(Object *thiz)
{
	SetThis(Player);

	thiz->polygon = newPolygon(thiz, LAYER_PLAYER, TEX_PLAYER);
	thiz->collider = newCollider(thiz);
	thiz->transform->position = Vector3(0.0f, 0.0f, 0.0f);
	thiz->transform->scale = Vector3(0.5f, 0.5f, 0.0f);

	thizz->hp = 3;
	thizz->speed = 700.0f;
	thizz->dir = Vector3(0, -1, 0);
	thizz->timer = 0;
	thizz->muteki = false;

}

void updatePlayer(Object *thiz)
{
	SetThis(Player);


	// �ړ�����
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

	// ��������
	float boost = 1.0f;
	//if (GetKeyboardPress(DIK_LSHIFT))
	//	boost = 2.0f;

	// �ړ�����
	thiz->transform->position += thizz->speed * boost * controlVector * GetDeltaTime();


	// �}�E�X�Ə��̌v�Z

	//if(GetMousePos().x > 0)
	//	thiz->transform->rotation.z = acosf(D3DXVec3Dot(&Vector3(0,-1,0), &GetMousePos()) / (D3DXVec3Length(&GetMousePos())));
	//else
	//	thiz->transform->rotation.z = PI+acosf(-D3DXVec3Dot(&Vector3(0, -1, 0), &GetMousePos()) / (D3DXVec3Length(&GetMousePos())));

	//D3DXVec3Normalize(&thizz->dir, &GetMousePos());
	//thiz->transform->rotation.z = atan2f(GetMousePos().y, GetMousePos().x) + PI / 2;

	// �e����
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

	// �e����
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
			newBullet(thiz->transform, 700.0f*thizz->dir + thizz->speed * boost * controlVector);
			thizz->timer = 0;
		}
		thizz->timer += GetDeltaTime();
	}

	// ���G��Ԃ̍X�V
	update_player_muteki(thizz);

}

void uninitPlayer(Object *thiz)
{

}

void onCollisionPlayer(Object * thiz, Object * other)
{
	SetThis(Player);

	if (other->type == Obj_Enemy && !thizz->muteki)
	{
		Polygon_SetOpacity(thiz->polygon, 0.5f);
		thizz->hp--;
		thizz->muteki = true;
		thizz->timer2 = 0;
	}
}

int Player_GetLives(Player * thiz)
{
	return 0;
}

void update_player_muteki(Player *thiz)
{
	if (!thiz->muteki)
		return;

	if (thiz->timer2 > 1.5f)
	{
		Polygon_SetOpacity(thiz->base->polygon, 1.0f);
		thiz->muteki = false;
		return;
	}



	thiz->timer2 += GetDeltaTime();
}
