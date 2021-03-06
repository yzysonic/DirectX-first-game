#pragma once
#include "Core/Core.h"

class PolygonElement : public Object
{
public:
	float targetOpacity;
	Vector3 prePos;
	Vector3 targetScale;
	Color nextColor;

	PolygonElement(void);
	void Update(void) override;

private:
	float timer;
	float timerInterval;
	Vector3 speed;
	void(PolygonElement::*pUpdate)();
	void update_state0();
	void update_state1();
};

