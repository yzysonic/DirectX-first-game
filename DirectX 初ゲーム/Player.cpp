#include "Player.h"
#include "SceneGlobal.h"

Player::Player()
{
	AddComponent<RectPolygon>("player", Layer::PLAYER);
	AddComponent<SphereCollider>()
		->radius = 25.0f;

	this->type = ObjectType::Player;
	this->transform.position = Vector3(0.0f, 0.0f, 0.0f);
	this->transform.scale = Vector3(0.5f, 0.5f, 0.0f);
	this->hp = 3;
	this->speed = 700.0f;
	this->boost = 1.0f;
	this->timer = 0;
	this->muteki = false;
	this->autoAim = false;
}

Player::~Player(void)
{
	StopSound(SE_LOW_HP);
}

void Player::Update()
{
	this->control = Vector3::zero;

	// 移動処理
	this->move();

	// 射撃処理
	this->shoot();

	// 向きの設定
	if ((control.x != 0.0f) || control.y != 0.0f)
		this->transform.setUp(control);

	// 無敵状態の更新
	this->update_muteki();
}

void Player::OnCollision(Object * other)
{
	if ((other->type == ObjectType::Enemy || other->type == ObjectType::Bullet_E) && !this->muteki)
	{
		GetComponent<RectPolygon>()->SetOpacity(0.5f);
		this->hp--;
		this->muteki = true;
		this->timer_flash =
		this->timer_muteki = 0;
		this->injury();
	}
}

void Player::update_muteki()
{
	static bool sw = false;

	if (!this->muteki)
		return;

	if (this->timer_flash > (0.07f - 0.04f*(this->timer_muteki/1.5f)) )
	{
		if(sw)
			GetComponent<RectPolygon>()->SetOpacity(1.0f);
		else
			GetComponent<RectPolygon>()->SetOpacity(0.3f);
		sw = !sw;
		this->timer_flash = 0.0f;
	}

	if (this->timer_muteki > 1.5f)
	{
		GetComponent<RectPolygon>()->SetOpacity(1.0f);
		this->muteki = false;
		return;
	}

	this->timer_flash += Time::DeltaTime();
	this->timer_muteki += Time::DeltaTime();
}

void Player::move(void)
{

	// キーボード入力
	if (GetKeyboardPress(DIK_W))
		control += Vector3(0.0f, 1.0f, 0.0f);
	if (GetKeyboardPress(DIK_S))
		control += Vector3(0.0f, -1.0f, 0.0f);
	if (GetKeyboardPress(DIK_A))
		control += Vector3(-1.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_D))
		control += Vector3(1.0f, 0.0f, 0.0f);

	// パッド入力
	control += Vector3(GetPadLX(), -GetPadLY(), 0);


	// 加速入力
	//if (GetKeyboardPress(DIK_LSHIFT))
	//	boost = 2.0f;

	// 移動処理
	if(control.sqrLength() > 0.0f)
		this->transform.position += control.normalized() * this->speed * this->boost * Time::DeltaTime();

}

void Player::shoot(void)
{
	Vector3 shoot_control;

	// キーボード入力
	if (GetKeyboardPress(DIK_UP))
		shoot_control += Vector3(0.0f, 1.0f, 0.0f);
	if (GetKeyboardPress(DIK_DOWN))
		shoot_control += Vector3(0.0f, -1.0f, 0.0f);
	if (GetKeyboardPress(DIK_LEFT))
		shoot_control += Vector3(-1.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_RIGHT))
		shoot_control += Vector3(1.0f, 0.0f, 0.0f);

	// パッド入力
	if (fabsf(GetPadRX()) > 0.1f || fabsf(GetPadRY() > 0.1f))
		shoot_control = Vector3(GetPadRX(), -GetPadRY(), 0);

	// マウス入力
	if (IsMouseLeftPressed())
		shoot_control = GetMousePos();

	// 自動照準
	//if (GetKeyboardTrigger(DIK_RSHIFT) || IsButtonTriggered(0, BUTTON_L1))
	//	this->autoAim = !this->autoAim;
	//if (this->autoAim)
	//{
	//	// TODO 自動照準
	//}

	//弾発射
	if ((shoot_control.x != 0.0f) || shoot_control.y != 0.0f)
	{
		shoot_control = shoot_control.normalized();

		if (this->timer >= 0.13f)
		{
			new Bullet(this, 1.7f * this->speed * shoot_control);
			this->timer = 0;
		}
		this->control = shoot_control;
		this->timer += Time::DeltaTime();
	}

}
