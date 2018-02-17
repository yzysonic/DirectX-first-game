#pragma once
#include "Core/Core.h"

#define BULLET_MAX (100)

class Bullet : public Object
{
private:
	static constexpr float LifeTime = 3.0f;
	static constexpr float WeakTime = 0.5f;

public:
	Bullet(Object* owner, Vector3 velocity);
	void Update(void) override;
	void OnDraw(void) override;
	void AfterDraw(void) override;
	void OnCollision(Object* other) override;

private:
	enum State
	{
		Normal,
		Collision,
		Weak
	}state;
	FrameTimer timer;

	void SetStateToCollision(void);
};