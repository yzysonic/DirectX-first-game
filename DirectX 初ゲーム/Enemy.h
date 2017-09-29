#pragma once
#include "Core.h"

#define ENEMY_SPEED (50)

class Enemy : public Object
{
public:
	Transform *target = NULL;
	int hp;
	float timer;

	Enemy(void);
	void update(void) override;
	void onCollision(Object* other) override;
};