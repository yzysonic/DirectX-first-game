#pragma once
#include "Core/Core.h"

class Enemy : public Object
{
private:
	static constexpr int MaxHP = 7;
	static constexpr float InitTime = 1.0f;
	static constexpr float Speed = 50.0f;
	static constexpr float EffectTime = 0.1f;

public:
	Transform *target = NULL;
	Event injury;
	int hp;
	float timer;
	float effect_timer;

	Enemy(void);
	void Update(void) override;
	void OnCollision(Object* other) override;
	void SetDeath(void);

private:
	void (Enemy::*pUpdate)(void);
	void update_init(void);
	void update_main(void);
	void update_death(void);
};