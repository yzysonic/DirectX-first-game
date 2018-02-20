#pragma once

#include "Core/Core.h"
#include "Bullet.h"

#define PlayerInitColor Color(180, 227, 255, 255)

class Player : public Object
{
private:
	static constexpr int MaxHP = 3;

public:
	enum State
	{
		Normal,
		Death,
		Vanish
	};

	int hp;
	State state;
	Event event_injury;
	Event event_move;
	Player(void);
	~Player(void);
	void Update(void) override;
	void OnCollision(Object* other) override;
	void SetDeath(void);

private:
	Vector3 control;
	float speed;
	float boost;
	FrameTimer timer;
	float timer_flash;
	float timer_muteki;

	bool muteki;
	bool autoAim;

	void update_muteki(void);
	void move(void);
	void shoot(void);
};