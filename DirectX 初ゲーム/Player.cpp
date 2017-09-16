#include "Player.h"

Player::Player()
{

	this->setPolygon(Layer::PLAYER, TEX_PLAYER);
	this->setCollider();
	this->transform->position = Vector3(0.0f, 0.0f, 0.0f);
	this->transform->scale = Vector3(0.5f, 0.5f, 0.0f);

	this->hp = 3;
	this->speed = 700.0f;
	this->dir = Vector3(0, -1, 0);
	this->timer = 0;
	this->muteki = false;

}

void Player::update()
{

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
		this->dir = controlVector;
		this->transform->rotation.z = atan2f(controlVector.y, controlVector.x) + PI / 2;
	}

	// ‰Á‘¬“ü—Í
	float boost = 1.0f;
	//if (GetKeyboardPress(DIK_LSHIFT))
	//	boost = 2.0f;

	// ˆÚ“®ˆ—
	this->transform->position += this->speed * boost * controlVector * Time::DeltaTime();


	// ƒ}ƒEƒXÆ€‚ÌŒvŽZ

	//if(GetMousePos().x > 0)
	//	this->transform->rotation.z = acosf(D3DXVec3Dot(&Vector3(0,-1,0), &GetMousePos()) / (D3DXVec3Length(&GetMousePos())));
	//else
	//	this->transform->rotation.z = PI+acosf(-D3DXVec3Dot(&Vector3(0, -1, 0), &GetMousePos()) / (D3DXVec3Length(&GetMousePos())));

	//D3DXVec3Normalize(&this->dir, &GetMousePos());
	//this->transform->rotation.z = atan2f(GetMousePos().y, GetMousePos().x) + PI / 2;

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
		this->dir = controlVector;
		this->transform->rotation.z = atan2f(controlVector.y, controlVector.x) + PI / 2;
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
		if(this->timer >= 0.13f)
		{
			new Bullet(this, 700.0f*this->dir + this->speed * boost * controlVector);
			this->timer = 0;
		}
		this->timer += Time::DeltaTime();
	}

	// –³“Gó‘Ô‚ÌXV
	this->update_muteki();

}

void Player::onCollision(Object * other)
{
	if (other->type == ObjectType::Enemy && !this->muteki)
	{
		this->polygon->setOpacity(0.5f);
		this->hp--;
		this->muteki = true;
		this->timer2 = 0;
	}
}

void Player::update_muteki()
{
	if (!this->muteki)
		return;

	if (this->timer2 > 1.5f)
	{
		this->polygon->setOpacity(1.0f);
		this->muteki = false;
		return;
	}

	this->timer2 += Time::DeltaTime();
}
