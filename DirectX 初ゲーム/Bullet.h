#pragma once
#include "Core/Core.h"

class Bullet : public Object
{
protected:
	static constexpr float LifeTime = 3.0f;
	static constexpr float WeakTime = 0.5f;

public:
	Bullet(Object* owner, Vector3 velocity);
	void Update(void) override;
	void OnDraw(void) override;
	void AfterDraw(void) override;
	void OnCollision(Object* other) override;

protected:
	enum State
	{
		Normal,
		Collision,
		Weak
	}state;
	FrameTimer timer;

	void SetStateToCollision(void);
};