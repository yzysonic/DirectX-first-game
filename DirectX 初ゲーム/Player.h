#pragma once

#include "Core/Core.h"
#include "Bullet.h"
#include "SmoothCamera.h"


class Player : public Object2D
{
public:
	int hp;
	bool shake_flag;
	Player(void);
	~Player(void);
	void update(void) override;
	void onCollision(Object2D* other) override;

private:
	Vector3 control;
	float speed;
	float boost;
	float timer;
	float timer_flash;
	float timer_muteki;

	bool muteki;
	bool autoAim;

	void update_muteki(void);
	void move(void);
	void shoot(void);
};