#include "Player.h"
#include "SceneGlobal.h"

Player::Player()
{
	AddComponent<RectPolygon>("player", Layer::PLAYER)->SetColor(PlayerInitColor);
	AddComponent<SphereCollider>()
		->radius = 25.0f;

	this->type = ObjectType::Player;
	this->transform.position = Vector3(0.0f, 0.0f, 0.0f);
	this->transform.scale = Vector3(0.5f, 0.5f, 0.0f);
	this->hp = MaxHP;
	this->speed = 700.0f;
	this->boost = 1.0f;
	this->timer.Reset(0.13f);
	this->muteki = false;
	this->autoAim = false;
	this->state = State::Normal;
}

Player::~Player(void)
{
	StopSound(SE_LOW_HP);
}

void Player::Update()
{
	switch (this->state)
	{
	case State::Normal:

		this->control = Vector3::zero;

		// �ړ�����
		this->move();

		// �ˌ�����
		this->shoot();

		// �����̐ݒ�
		if ((control.x != 0.0f) || control.y != 0.0f)
			this->transform.setUp(control);

		// ���G��Ԃ̍X�V
		this->update_muteki();

		break;

	case State::Death:

		if (this->timer.TimeUp())
			this->state = State::Vanish;

		this->timer++;

		break;
	}
	
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
		this->event_injury();
	}
}

void Player::SetDeath(void)
{
	this->GetComponent<RectPolygon>()->SetActive(false);
	this->GetComponent<SphereCollider>()->SetActive(false);

	auto particle =
	this->AddComponent<ParticleSystem>(3000);
	particle->SetDuration(0.3f);
	particle->emission_rate = 30000.0f;

	auto behavior = particle->GetBehavior<ParticleDefaultBehavior>();
	behavior->start_color = Color(207, 240, 243, 255);
	behavior->start_size = 5.0f;
	behavior->start_speed = 10.0f;
	behavior->end_size = 0.0f;
	//behavior->end_color = Color(255, 255, 255, 255);
	behavior->end_opacity = 0.5f;

	this->timer.Reset(3.0f);

	this->state = State::Death;
}

void Player::update_muteki()
{
	static bool sw = false;

	if (!this->muteki)
		return;

	float x = (Lerpf((float)hp, (float)hp-1, this->timer_muteki/ 0.3f*1.5f) / MaxHP);
	UCHAR r, g, b;

	r = (UCHAR)(PlayerInitColor.r * x + 255 * (1.0f - x));
	g = (UCHAR)(PlayerInitColor.g * x + 255 * (1.0f - x));
	b = (UCHAR)(PlayerInitColor.b * x + 255 * (1.0f - x));

	GetComponent<RectPolygon>()->SetColor(Color(r, g, b, 255));

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

	// �L�[�{�[�h����
	if (GetKeyboardPress(DIK_W))
		control += Vector3(0.0f, 1.0f, 0.0f);
	if (GetKeyboardPress(DIK_S))
		control += Vector3(0.0f, -1.0f, 0.0f);
	if (GetKeyboardPress(DIK_A))
		control += Vector3(-1.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_D))
		control += Vector3(1.0f, 0.0f, 0.0f);

	if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D))
		control = control.normalized();

	// �p�b�h����
	control += Vector3(GetPadLX(), -GetPadLY(), 0);

	// �ړ�����
	if (control.sqrLength() > 0.0f)
	{
		this->transform.position += control * this->speed * this->boost * Time::DeltaTime();
		this->event_move();
	}


}

void Player::shoot(void)
{
	Vector3 shoot_control;

	// �L�[�{�[�h����
	if (GetKeyboardPress(DIK_UP))
		shoot_control += Vector3(0.0f, 1.0f, 0.0f);
	if (GetKeyboardPress(DIK_DOWN))
		shoot_control += Vector3(0.0f, -1.0f, 0.0f);
	if (GetKeyboardPress(DIK_LEFT))
		shoot_control += Vector3(-1.0f, 0.0f, 0.0f);
	if (GetKeyboardPress(DIK_RIGHT))
		shoot_control += Vector3(1.0f, 0.0f, 0.0f);

	// �p�b�h����
	if (fabsf(GetPadRX()) > 0.1f || fabsf(GetPadRY() > 0.1f))
		shoot_control = Vector3(GetPadRX(), -GetPadRY(), 0);

	// �}�E�X����
	if (IsMouseLeftPressed())
		shoot_control = GetMousePos();

	// �����Ə�
	//if (GetKeyboardTrigger(DIK_RSHIFT) || IsButtonTriggered(0, BUTTON_L1))
	//	this->autoAim = !this->autoAim;
	//if (this->autoAim)
	//{
	//	// TODO �����Ə�
	//}

	//�e����
	if ((shoot_control.x != 0.0f) || shoot_control.y != 0.0f)
	{
		shoot_control = shoot_control.normalized();

		if (this->timer.TimeUp())
		{
			/*this->scene->AddObject*/(new Bullet(this, 1.7f * this->speed * shoot_control));
			this->timer.Reset(0.13f);
		}
		this->control = shoot_control;
		this->timer ++;
	}

}
