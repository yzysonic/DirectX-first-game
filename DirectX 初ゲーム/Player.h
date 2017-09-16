#pragma once

#include"Core.h"
#include "Bullet.h"


class Player : public Object
{
public:
	int hp;

	Player(void);
	void update(void) override;
	void onCollision(Object* other) override;
private:
	Vector3 dir;
	float speed;
	float timer;
	float timer2;
	bool muteki;

	void update_muteki(void);
};