#pragma once

#include "Core/Core.h"
#include "Bullet.h"


class Player : public Object
{
public:
	int hp;
	Event injury;
	Player(void);
	~Player(void);
	void Update(void) override;
	void OnCollision(Object* other) override;

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