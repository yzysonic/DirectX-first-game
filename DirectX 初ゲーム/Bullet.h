#pragma once
#include "Core/Core.h"
#

#define BULLET_MAX (100)

class Bullet : public Object
{
public:
	Bullet(Object* owner, Vector3 velocity);
	~Bullet(void);
	void Update(void) override;
	void OnCollision(Object* other) override;
	static void Clear();
private:
	static std::list<Bullet*> list;
	float timer;
	int index;
};