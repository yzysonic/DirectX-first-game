#include "Player.h"

Player::Player()
{
	this->type = ObjectType::Player;
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

	// 移動入力
	Vector3 controlVector = Vector3(0.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_W))
	{
		controlVector += Vector3(0.0f, -1.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_S))
	{
		controlVector += Vector3(0.0f, 1.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_A))
	{
		controlVector += Vector3(-1.0f, 0.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_D))
	{
		controlVector += Vector3(1.0f, 0.0f, 0.0f);
	}

	if ((controlVector.x != 0.0f) || controlVector.y != 0.0f)
	{
		controlVector = controlVector.normalized();
		this->dir = controlVector;
		this->transform->rotation.z = atan2f(controlVector.y, controlVector.x) + PI / 2;
	}

	// 加速入力
	float boost = 1.0f;
	//if (GetKeyboardPress(DIK_LSHIFT))
	//	boost = 2.0f;

	// 移動処理
	this->transform->position += controlVector * this->speed * boost * Time::DeltaTime();


	// マウス照準の計算

	//if(GetMousePos().x > 0)
	//	this->transform->rotation.z = acosf(D3DXVec3Dot(&Vector3(0,-1,0), &GetMousePos()) / (D3DXVec3Length(&GetMousePos())));
	//else
	//	this->transform->rotation.z = PI+acosf(-D3DXVec3Dot(&Vector3(0, -1, 0), &GetMousePos()) / (D3DXVec3Length(&GetMousePos())));

	//D3DXVec3Normalize(&this->dir, &GetMousePos());
	//this->transform->rotation.z = atan2f(GetMousePos().y, GetMousePos().x) + PI / 2;

	// 弾発射
	controlVector = Vector3(0.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_UP))
	{
		controlVector += Vector3(0.0f, -1.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		controlVector += Vector3(0.0f, 1.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		controlVector += Vector3(-1.0f, 0.0f, 0.0f);
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		controlVector += Vector3(1.0f, 0.0f, 0.0f);
	}

	if ((controlVector.x != 0.0f) || controlVector.y != 0.0f)
	{
		controlVector = controlVector.normalized();
		this->dir = controlVector;
		this->transform->rotation.z = atan2f(controlVector.y, controlVector.x) + PI / 2;
	}

	// 弾発射
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

	// 無敵状態の更新
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
