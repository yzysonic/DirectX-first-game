#pragma once
#include "Core/Core.h"

class Enemy : public Object2D
{
private:
	static constexpr int MaxHP = 3;
	static constexpr float InitTime = 1.0f;
	static constexpr float Speed = 50.0f;
	static constexpr float EffectTime = 0.1f;

public:
	Transform *target = NULL;
	int hp;
	float timer;
	float effect_timer;

	Enemy(void);
	void update(void) override;
	void onCollision(Object2D* other) override;
private:
	void (Enemy::*pUpdate)(void);
	void update_init(void);
	void update_main(void);
};