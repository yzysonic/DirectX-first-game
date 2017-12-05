#pragma once
#include "Core/Core.h"

#define ENEMY_SPEED (50)

class Enemy : public Object2D
{
public:
	Transform *target = NULL;
	int hp;
	float timer;

	Enemy(void);
	void update(void) override;
	void onCollision(Object2D* other) override;
};