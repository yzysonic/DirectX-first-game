#pragma once
#include "Core\Core.h"

class FieldBoundary : public Object
{
public:
	FieldBoundary(float width, float height);
	void Update(void) override;
	void Touch(void);

private:
	int state;
	RectPolygon2D* poly;
	FrameTimer timer;
};